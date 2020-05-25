.PHONY: clean All

All:
	@echo "----------Building project:[ linux_rtt - Simulator ]----------"
	@"$(MAKE)" -f  "linux_rtt.mk" PreBuild && "$(MAKE)" -f  "linux_rtt.mk" && "$(MAKE)" -f  "linux_rtt.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ linux_rtt - Simulator ]----------"
	@"$(MAKE)" -f  "linux_rtt.mk" clean
