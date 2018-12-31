.PHONY: clean All

All:
	@echo "----------Building project:[ linux_rtt - Debug ]----------"
	@"$(MAKE)" -f  "linux_rtt.mk"
clean:
	@echo "----------Cleaning project:[ linux_rtt - Debug ]----------"
	@"$(MAKE)" -f  "linux_rtt.mk" clean
