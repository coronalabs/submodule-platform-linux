.PHONY: clean All

All:
	@echo "----------Building project:[ Solar2DSimulator - Debug ]----------"
	@"$(MAKE)" -f  "Solar2DSimulator.mk" PreBuild && "$(MAKE)" -f  "Solar2DSimulator.mk" && "$(MAKE)" -f  "Solar2DSimulator.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ Solar2DSimulator - Debug ]----------"
	@"$(MAKE)" -f  "Solar2DSimulator.mk" clean
