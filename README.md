## How To Build For Native Linux

If you have not done it already you will need to clone the git repository

```
cd ~
git clone --recursive https://github.com/coronalabs/corona.git 
```

Run the provided script to install required dependencies.
```
cd ~/corona/platform/linux
sudo sh ./setup_dev.sh  
```

For those who like IDE's the source has been setup for use with CodeLite it can be installed using.
```
sudo apt-get install codelite -y
```

If you have installed CodeLite the workspace file is named "corona.workspace" and is located at ~/corona/platform/linux



## Building Linux templates

Linux relies on [Docker](https://docs.docker.com/docker-for-mac/) for mac to build it's templates.
After files are built, replace `linuxtemplate.tar.gz` and `raspbiantemplate.tar.gz` to build apps with
your custom code.

To build everything run. This will build templates and linux simulator.

```
./build_template.sh

```

You would also need Raspbian binaries to build cross-compiled template for Raspbian Pi. You can download them [here](https://drive.google.com/file/d/1ZysxJdDg-XgU3-jshxUPSewTqBYeA7Qq/view?usp=sharing).

## Details

Note, that `build_template.sh` runs on macOS, while `build_template_linux.sh` runs inside Docker container on Linux. You can comment out parts of the script to make build faster.
