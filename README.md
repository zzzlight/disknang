**build**:

nohup ./build_disk_index uint8 ~/ANNS/zmq/Disk/sift_10e.bin test_10e 100 64 40 40 48 1>10e_build.log & 



**search:**

nohup ./search_disk_index ~/ANNS/zmq/Disk/sift_query.fvecs 165 100 test_10e ~/ANNS/yq/dataset/origin/bigann/gnd/idx_1000M.ivecs 40 10e_result 100 4 1>10e_search_result.log  2>&1 &



