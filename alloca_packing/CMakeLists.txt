add_llvm_library(LLVMJPT MODULE
    # List your source files here.
    alloca_packing.cpp
    PLUGIN_TOOL
    opt
)

#set_target_properties(StatisticsPass PROPERTIES
#    COMPILE_FLAGS "-fno-rtti"
#)
