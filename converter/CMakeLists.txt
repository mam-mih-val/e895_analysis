add_executable(convert
        src/main.cc
        src/converter.cc
        src/input_chain.cc
        src/out_tree_manager.cc)
target_link_libraries(convert ROOT::RIO ROOT::Tree AnalysisTreeBase AnalysisTreeInfra Boost::program_options)