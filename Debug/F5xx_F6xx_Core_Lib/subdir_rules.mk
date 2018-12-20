################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
F5xx_F6xx_Core_Lib/%.obj: ../F5xx_F6xx_Core_Lib/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv8/ccs_base/msp430/include" --include_path="D:/CCS_workspace/HardWareFinalWork_DAC_Output" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.4.LTS/include" --advice:power=all --define=__MSP430F6638__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="F5xx_F6xx_Core_Lib/$(basename $(<F)).d_raw" --obj_directory="F5xx_F6xx_Core_Lib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


