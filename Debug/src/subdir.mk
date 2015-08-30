################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CapacitorsChipSMD.cpp \
../src/CapacitorsElectrolyticSMD.cpp \
../src/CapacitorsPolarizedSMD.cpp \
../src/DualFlatNoLead.cpp \
../src/DualInLine.cpp \
../src/Footprint.cpp \
../src/KiCad_Footprint_Generator.cpp \
../src/PinHeadersAngled.cpp \
../src/PinHeadersStraight.cpp \
../src/QuadFlatNoLead.cpp \
../src/QuadFlatPackage.cpp \
../src/ResistorsChipSMD.cpp \
../src/SmallOutlinePackage.cpp \
../src/SocketStripsAngled.cpp \
../src/SocketStripsStraight.cpp 

OBJS += \
./src/CapacitorsChipSMD.o \
./src/CapacitorsElectrolyticSMD.o \
./src/CapacitorsPolarizedSMD.o \
./src/DualFlatNoLead.o \
./src/DualInLine.o \
./src/Footprint.o \
./src/KiCad_Footprint_Generator.o \
./src/PinHeadersAngled.o \
./src/PinHeadersStraight.o \
./src/QuadFlatNoLead.o \
./src/QuadFlatPackage.o \
./src/ResistorsChipSMD.o \
./src/SmallOutlinePackage.o \
./src/SocketStripsAngled.o \
./src/SocketStripsStraight.o 

CPP_DEPS += \
./src/CapacitorsChipSMD.d \
./src/CapacitorsElectrolyticSMD.d \
./src/CapacitorsPolarizedSMD.d \
./src/DualFlatNoLead.d \
./src/DualInLine.d \
./src/Footprint.d \
./src/KiCad_Footprint_Generator.d \
./src/PinHeadersAngled.d \
./src/PinHeadersStraight.d \
./src/QuadFlatNoLead.d \
./src/QuadFlatPackage.d \
./src/ResistorsChipSMD.d \
./src/SmallOutlinePackage.d \
./src/SocketStripsAngled.d \
./src/SocketStripsStraight.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


