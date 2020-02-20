//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md 
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#include "Core/Rtt_Build.h"
#include "Rtt_LinuxImageProvider.h"
#include "Rtt_LinuxBitmap.h"
#include "Display/Rtt_BitmapPaint.h"
#include "Display/Rtt_LuaLibDisplay.h"
#include "Rtt_Lua.h"
#include "Rtt_LuaContext.h"
#include "Display/Rtt_Display.h"

#ifndef _WIN32

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

//
// NOT COMPLETED YET !!!
//

namespace Rtt
{ 

	enum io_method
	{
		IO_METHOD_READ,
		IO_METHOD_MMAP,
		IO_METHOD_USERPTR,
	};

	#define CLEAR(x) memset(&(x), 0, sizeof(x))

	static const char* dev_name = "/dev/video0";
	static enum io_method   io = IO_METHOD_MMAP;
	
struct buffer
{
	void   *start;
	size_t  length;
};
struct buffer* buffers;	
static unsigned int n_buffers;

	/// Creates a new image provider.
	/// @param handle Reference to Lua state used to send image selection notifications to a Lua listener function.
	LinuxImageProvider::LinuxImageProvider(const ResourceHandle<lua_State> & handle, int w, int h)
		: PlatformImageProvider(handle)
		, fd(-1)
	{
		if (open_device())
		{
			init_device();
		}
	} 

	LinuxImageProvider::~LinuxImageProvider()
	{
		uninit_device();
		close_device();
	}

	bool LinuxImageProvider::open_device()
	{
		struct stat st;
		if (stat(dev_name, &st) == -1)
		{
			Rtt_LogException("Cannot identify '%s': errno: %d, %s\n", dev_name, errno, strerror(errno));
			return false;
		}

		if (S_ISCHR(st.st_mode) == false)
		{
			Rtt_LogException("%s is no device\n", dev_name);
			return false;
		}

		fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
		if (fd == -1)
		{
			Rtt_LogException("Cannot open '%s': %d, %s\n", dev_name, errno, strerror(errno));
			return false;
		}
		return true;
	}

	void LinuxImageProvider::close_device()
	{
		if (fd != -1 && close(fd) == -1)
		{
			Rtt_LogException("close webcam failed");
		}
		fd = -1;
	}

	int LinuxImageProvider::xioctl(int fh, unsigned long int request, void *arg)
	{
		int r;
		do {
			r = ioctl(fh, request, arg);
		}
		while (r == -1 && EINTR == errno);
		return r;
	}

	void LinuxImageProvider::init_read(unsigned int buffer_size)
	{
		buffers = (buffer*) calloc(1, sizeof(*buffers));

		if (!buffers) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}

		buffers[0].length = buffer_size;
		buffers[0].start = malloc(buffer_size);

		if (!buffers[0].start) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
	}

	bool LinuxImageProvider::init_mmap()
	{
		struct v4l2_requestbuffers req;

		CLEAR(req);

		req.count = 4;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_MMAP;

		if (xioctl(fd, VIDIOC_REQBUFS, &req) == -1) 
		{
			if (EINVAL == errno)
			{
				Rtt_LogException("%s does not support memory mapping\n", dev_name);
				return false;
			} 
			else
			{
				Rtt_LogException("VIDIOC_REQBUFS failed");
				return false;
			}
		}

		if (req.count < 2)
		{
			Rtt_LogException("Insufficient buffer memory on %s\n", dev_name);
			return false;
		}

		buffers = (buffer*) calloc(req.count, sizeof(*buffers));
		if (buffers == NULL)
		{
			Rtt_LogException("Out of memory\n");
			return false;
		}

		for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
		{
			struct v4l2_buffer buf;

			CLEAR(buf);

			buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory      = V4L2_MEMORY_MMAP;
			buf.index       = n_buffers;

			if (xioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)
			{
				Rtt_LogException("VIDIOC_QUERYBUF failed");
				return false;
			}

			buffers[n_buffers].length = buf.length;
			buffers[n_buffers].start =
				mmap(NULL, // start anywhere
							buf.length,
							PROT_READ | PROT_WRITE,
							MAP_SHARED,  // recommended
							fd, buf.m.offset);

			if (MAP_FAILED == buffers[n_buffers].start)
			{
				Rtt_LogException("mmap failed");
				return false;
			}
		}
		return true;
	}

	void LinuxImageProvider::init_userp(unsigned int buffer_size)
	{
		struct v4l2_requestbuffers req;

		CLEAR(req);

		req.count  = 4;
		req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_USERPTR;

		if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
			if (EINVAL == errno) {
				fprintf(stderr, "%s does not support "
					 "user pointer i/o\n", dev_name);
				exit(EXIT_FAILURE);
			} else {
				printf("VIDIOC_REQBUFS");
			}
		}

		buffers = (buffer*) calloc(4, sizeof(*buffers));

		if (!buffers) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}

		for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
			buffers[n_buffers].length = buffer_size;
			buffers[n_buffers].start = malloc(buffer_size);

			if (!buffers[n_buffers].start) {
				fprintf(stderr, "Out of memory\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	bool LinuxImageProvider::init_device()
	{
		struct v4l2_capability cap;
		struct v4l2_cropcap cropcap;
		struct v4l2_crop crop;
		struct v4l2_format fmt;

		if (xioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) 
		{
			if (errno == EINVAL) 
			{
				Rtt_LogException("%s is no V4L2 device\n", dev_name);
			} 
			else 
			{
				Rtt_LogException("VIDIOC_QUERYCAP failed");
			}
			return false;
		}

		if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE == 0)
		{
			Rtt_LogException("%s is no video capture device\n", dev_name);
			return false;
		}

		switch (io)
		{
			case IO_METHOD_READ:
				if (cap.capabilities & V4L2_CAP_READWRITE == 0)
				{
					Rtt_LogException("%s does not support read i/o\n", dev_name);
					return false;
				}
				break;

			case IO_METHOD_MMAP:
			case IO_METHOD_USERPTR:
				if (cap.capabilities & V4L2_CAP_STREAMING == 0)
				{
					Rtt_LogException("%s does not support streaming i/o\n", dev_name);
					return false;
				}
				break;
		}

		// Select video input, video standard and tune here.


		CLEAR(cropcap);

		cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (xioctl(fd, VIDIOC_CROPCAP, &cropcap) == 0)
		{
			crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			crop.c = cropcap.defrect; // reset to default 

			if (xioctl(fd, VIDIOC_S_CROP, &crop) == -1) 
			{
				switch (errno)
				{
					case EINVAL:
					// Cropping not supported.
					break;
					default:
						// Errors ignored. 
					break;
				}
			}
		}
		else
		{
			// Errors ignored.
		}


		CLEAR(fmt);

		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (force_format)
		{
			fmt.fmt.pix.width       = 640;
			fmt.fmt.pix.height      = 480;
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
			fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

			if (xioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
			{
				Rtt_LogException("VIDIOC_S_FMT failed");
				return false;
			}
			// Note VIDIOC_S_FMT may change width and height. 
		}
		else
		{
			// Preserve original settings as set by v4l2-ctl for example 
			if (xioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
			{
				Rtt_LogException("VIDIOC_G_FMT filed");
				return false;
			}
		}

		bool rc = false;
		switch (io)
		{
			case IO_METHOD_READ:
				init_read(fmt.fmt.pix.sizeimage);
				break;

			case IO_METHOD_MMAP:
				rc = init_mmap();
				break;

			case IO_METHOD_USERPTR:
				init_userp(fmt.fmt.pix.sizeimage);
				break;
		}
		return rc;
	}

	void LinuxImageProvider::uninit_device()
	{
		unsigned int i;
		switch (io)
		{
		case IO_METHOD_READ:
			free(buffers[0].start);
			break;

		case IO_METHOD_MMAP:
			for (i = 0; i < n_buffers; ++i)
			{
				if (munmap(buffers[i].start, buffers[i].length) == -1)
				{
					Rtt_LogException("munmap failed");
					break;
				}
			}
			break;

		case IO_METHOD_USERPTR:
			for (i = 0; i < n_buffers; ++i)
			{
				free(buffers[i].start);
			}
			break;
		}
		free(buffers);
	}

	/// Determines if the given image source (Camera, Photo Library, etc.) is supported on this platform.
	/// @param source Unique integer ID of the image source. IDs are defined in PlatformImageProvider class.
	/// @return Returns true if given image source is supported on this platform. Returns false if not.
	bool LinuxImageProvider::Supports(int source) const
	{
		bool hasSource = false;
		switch (source)
		{
			case IMAGE_SOURCE_PHOTO_LIBRARY:
			case IMAGE_SOURCE_SAVED_PHOTOS_ALBUM:
				hasSource = false;
				break;
			case IMAGE_SOURCE_CAMERA:
				hasSource = fd != -1;
				break;
		}
		return hasSource;
	}

	/// Displays a window for selecting an image.
	/// @param source Unique integer ID indicating what kind of window to display such as the Camera or Photo Library.
	/// @param filePath The path\file name to save the selected image file to.
	///                 Set to NULL to not save to file and display the selected photo as a display object instead.
	/// @return Returns true if the window was shown.
	///         Returns false if given source is not provided or if an image provider window is currently shown.
	// PlatformImageProvider::kCamera = 1
	bool LinuxImageProvider::Show(int source, const char* filePath, lua_State* L)
	{
		// Do not continue if:
		// 1) Given image source type is not supported on this platform.
		// 2) Image providing window is already shown.

		PlatformImageProvider::Source src = (PlatformImageProvider::Source) source;

		// If given file path is an empty string, then change it to NULL.
		if (filePath && (strlen(filePath) <= 0))
		{
			filePath = NULL;
		}

		if (fd < 0)		// streamer created ?
		{
			Rtt_LogException("LinuxImageProvider is not implememnted\n");
			return false; 
		}
		return true;
	}

	void LinuxImageProvider::dispatch(LinuxVideoEventTypes eventID, int w, int h, uint8_t* buf)
	{
		//printf("event: id=%d, w=%d, h=%d, buf=%p,fObjID=%d\n", eventID, w, h, buf, fObjID);

		if (eventID == onCreated)
		{
//			fObjID = w;
			return;
		}

		lua_State *L = GetLuaState();
		LinuxImageProviderCompletionEvent e(w, h, buf);
		Runtime* runtime = LuaContext::GetRuntime(L);
		e.Dispatch(L, *runtime);

		if (fd < 0)
		{
//			jsImageProviderHide(fObjID);
			Rtt_LogException("LinuxImageProvider is not implememnted\n");
		}
	}

	int LinuxImageProvider::LinuxImageProviderCompletionEvent::Push(lua_State *L) const
	{
		if (Rtt_VERIFY(Super::Push(L)) && fData && fWidth > 0 && fHeight > 0)
		{
			lua_pushboolean(L, true);
			lua_setfield(L, -2, "completed");

			Runtime* runtime = LuaContext::GetRuntime(L);
			LinuxBaseBitmap* bitmap = new LinuxBaseBitmap(runtime->GetAllocator(), fWidth, fHeight, fData);
			BitmapPaint* paint = BitmapPaint::NewBitmap(runtime->GetDisplay().GetTextureFactory(), bitmap, false);
			LuaLibDisplay::PushImage(L, NULL, paint, runtime->GetDisplay(), NULL);
			lua_setfield(L, -2, "target");
		}
		return 1;
	}

} // namespace Rtt

#endif
