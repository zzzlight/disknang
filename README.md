**build**:

nohup ./build_disk_index uint8 ~/ANNS/zmq/Disk/sift_1e.bin test_1e 15 64 40 40 64 1>1e_build.log &

test1e后面第一个参数现在为iter 这里设置为15，最后一个参数为thread 这里设置为64，其他暂时没用



**search:**

nohup ./search_disk_index ~/ANNS/zmq/Disk/sift_query.fvecs 165 100 test_10e ~/ANNS/yq/dataset/origin/bigann/gnd/idx_1000M.ivecs 40 10e_result 100 4 1>10e_search_result.log  2>&1 &



