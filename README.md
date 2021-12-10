build:

nohup ./build_disk_index uint8 ~/ANNS/zmq/Disk/sift_base_10kw_correct.bin test_set3 60 75 120 120 90 1>1e_multi_newset3_single.log & 



search:

./test_disknang_search ./test_set1_mem.index_tempFiles_subshard-2.bin  ~/ANNS/zmq/Disk/sift_query.fvecs test_set1_disk.index2 165 100 sift_dng.result ~/ANNS/yq/dataset/origin/bigann/gnd/idx_100M.ivecs 100

