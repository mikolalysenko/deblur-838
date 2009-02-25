function run_mvflowgcut_float( af_filenum )

system(sprintf('"/afs/cs.wisc.edu/u/t/u/tuowang/Visual Studio 2005/Projects/VideoStab_V3_RefOutOf25_Final/planesweep/planesweep" -af /afs/cs.wisc.edu/u/t/u/tuowang/test/stabBatch/input_files/af_psweep_linux%03d.txt', af_filenum));

return;
