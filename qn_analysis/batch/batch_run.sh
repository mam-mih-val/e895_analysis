#!/bin/bash
#$ -l h=!(ncx182.jinr.ru|ncx211.jinr.ru)

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SGE_TASK_ID))
filelist=$lists_dir/$(ls $lists_dir | sed "${job_num}q;d")

cd $output_dir
mkdir -p $job_num
cd $job_num

source /cvmfs/nica.jinr.ru/sw/os/login.sh
module add GCC-Toolchain/

source /scratch1/mmamaev/bmn_environment.sh
echo
date
echo "Running conversion ..."
echo "/scratch1/mmamaev/e895_analysis/build/converter/convert -i $filelist -o output.root --output-tree atree -s $colliding_system -e $collision_energy"

/scratch1/mmamaev/e895_analysis/build/converter/convert \
                                                        -i $filelist \
                                                        -o output.root \
                                                        --output-tree atree \
                                                        -s $colliding_system \
                                                        -e $collision_energy

find . -name "output*.root" > list.txt
echo; date
#echo "Running QA step ..."
#input_list=\"list.txt\"
#output_qa=\"qa.root\"
#root -q "/scratch1/mmamaev/mcpico_to_analysis_tree/macro/run_qa.cc($input_list, $output_qa)"
#echo; date
echo "Running correction ..."
/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrect/QnAnalysisCorrect \
                                                                          -i list.txt \
                                                                          -t atree \
                                                                          -o correction_out.root \
                                                                          --yaml-config-file /scratch1/mmamaev/e895_analysis/qn_analysis/setups/e895_corrections.yml \
                                                                          --yaml-config-name E895
mv correction_out.root correction_in.root
/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrect/QnAnalysisCorrect \
                                                                          -i list.txt \
                                                                          -t atree \
                                                                          -o correction_out.root \
                                                                          --yaml-config-file /scratch1/mmamaev/e895_analysis/qn_analysis/setups/e895_corrections.yml \
                                                                          --yaml-config-name E895
mv correction_out.root correction_in.root
/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrect/QnAnalysisCorrect \
                                                                          -i list.txt \
                                                                          -t atree \
                                                                          -o correction_out.root \
                                                                          --yaml-config-file /scratch1/mmamaev/e895_analysis/qn_analysis/setups/e895_corrections.yml \
                                                                          --yaml-config-name E895
echo; date
echo "Running correlation ..."
/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrelate/QnAnalysisCorrelate \
                                                                              --input-file correction_out.root \
                                                                              --configuration-file /scratch1/mmamaev/e895_analysis/qn_analysis/setups/e895_correlations.yml \
                                                                              --configuration-name _tasks \
                                                                              -o correlation_out.root

echo PROCESS FINISHED