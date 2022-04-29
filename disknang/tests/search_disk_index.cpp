//
// Created by 付聪 on 2017/6/21.
// Modified by WMZ on 2020/6/16.
//Modified by zmq on 2021/11.
//

#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include<ctime>
#include "efanna2e/index_random.h"
#include "efanna2e/index_graph.h"
#include "efanna2e/util.h"
#include <omp.h>


void load_data(char* filename, int*& data, int& num,int& dim){// load data with sift10K pattern
  std::ifstream in(filename, std::ios::binary);
  if(!in.is_open()){std::cout<<"open file error"<<std::endl;return;}
  in.read((char*)&dim,4);
  in.seekg(0,std::ios::end);
  std::ios::pos_type ss = in.tellg();
  int fsize = (int)ss;
  num = fsize / (dim+1) / 4;
  data = new int[num*dim];

  in.seekg(0,std::ios::beg);
  for(int i = 0; i < num; i++){
    in.seekg(4,std::ios::cur);
    in.read((char*)(data+i*dim),dim*4);
  }
  in.close();
}


void save_result(char* filename, std::vector<std::vector<unsigned> >& results) {
  std::ofstream out(filename, std::ios::binary | std::ios::out);

  for (unsigned i = 0; i < results.size(); i++) {
    unsigned GK = (unsigned)results[i].size();
    
    out.write((char*)&GK, sizeof(unsigned));
    out.write((char*)results[i].data(), GK * sizeof(unsigned));
  }
  out.close();
}

bool cmp(efanna2e::IndexGraph::pointdata a,efanna2e::IndexGraph::pointdata b)
{
   return a.distance<b.distance;
}

int main(int argc, char** argv) {
  // if (argc != 9) {
  //   std::cout << "./run data_file query_file dng_path L K result_path ground_path topK"
  //             << std::endl;
  //   exit(-1);
  // }
  unsigned NUM_THREADS = 0;
  NUM_THREADS=atoi(argv[9]);
  if (NUM_THREADS != 0) omp_set_num_threads(NUM_THREADS);
  else omp_set_num_threads(4);
  srand(unsigned(time(0)));
  std::cerr << "Using time Seed " << std::endl;

  unsigned query_num, query_dim;
  float* query_load = nullptr;
  efanna2e::load_data(argv[1], query_load, query_num, query_dim);
  //assert(dim == query_dim);

  std::cerr << "query data Path: " << argv[1] << std::endl;
  unsigned L = (unsigned)atoi(argv[2]);
  unsigned K = (unsigned)atoi(argv[3]);

  std::cerr << "L = " << L << ", ";
  std::cerr << "K = " << K << std::endl;

  efanna2e::Parameters paras;
  paras.Set<unsigned>("L_search", L);
  std::cerr << "index part Path: " << argv[4] << std::endl;
  std::cerr << "ground truth Path: " << argv[5] << std::endl;
  
  std::vector<std::vector<efanna2e::IndexGraph::pointdata> > res(query_num);
  for (unsigned i = 0; i < query_num; i++) res[i].resize(K);

  std::vector<std::vector<unsigned> > new_res(query_num);
  for(unsigned i=0;i<query_num;i++) new_res[i].resize(K);
  // Warm up
  // for (int loop = 0; loop < 3; ++loop) {
  //   for (unsigned i = 0; i < 10; ++i) {
  //     index.MySearchWithOptGraph(query_load + i * dim, K, paras, res[i].data(),0);
  //   }
  // }

  std::vector<std::vector<efanna2e::IndexGraph::pointdata> > total_record(query_num);
  
  
  int parts=0;
  parts=atoi(argv[6]);
  unsigned offset=0;
  unsigned totalCount=0;

  auto s = std::chrono::high_resolution_clock::now();
  for(int i=0;i<parts;i++)
  {
      unsigned points_num, dim;
      std::string index_prefix_path=argv[4];
      std::string temp_data_file=index_prefix_path + "_mem.index"+ "_tempFiles"+ "_subshard-" + std::to_string(i) + ".bin";
      std::string temp_index_file="final"+std::to_string(i)+"_"+index_prefix_path+"_disk.index";
      float* data_load2 = nullptr;
      efanna2e::new_load_data(temp_data_file.c_str(), data_load2, points_num, dim);
      efanna2e::IndexRandom init_index(dim, points_num);
      efanna2e::IndexGraph index(dim, points_num, efanna2e::FAST_L2,(efanna2e::Index*)(&init_index));
      index.Load(temp_index_file.c_str());
      index.OptimizeGraph(data_load2);
      #pragma omp parallel
      {
        #pragma omp for schedule(dynamic, 100)
        for (unsigned i = 0; i < query_num; i++) res[i].resize(K);

        #pragma omp for schedule(dynamic, 100)
        for(unsigned i=0;i<query_num;i++) res[i].clear();
    
        #pragma omp for schedule(dynamic, 100)
        for (unsigned i = 0; i < query_num; i++) {
            index.MySearchWithOptGraph(query_load + i * dim, K, paras, res[i].data(),offset);
        }
    
        //std::cout<<"flag -------------------------------------------------------"<<std::endl;
        #pragma omp for schedule(dynamic, 100)
        for(unsigned x=0;x<query_num;x++){
            for(unsigned y=0;y<K;y++)
            {
              total_record[x].push_back(res[x][y]);
            }
        }
        #pragma omp for schedule(dynamic, 100)
        for(unsigned j=0;j<K;j++) std::vector<efanna2e::IndexGraph::pointdata>().swap(res[j]);
      }
      totalCount+=index.GetDistCount();
      offset+=25000000;
    }
  
    auto e1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff1 = e1 - s;
    std::cerr << "Search Time diff1: " << diff1.count() << std::endl;
    for(unsigned x=0;x<query_num;x++)
    {
      std::sort(total_record[x].begin(),total_record[x].end(),cmp);
      //use to debug
      // if(x==0) 
      // {
      //   std::cout<<"record is:"<<std::endl;
      //   for(unsigned t=0;t<total_record[0].size();t++)
      //   {
      //     std::cout<<total_record[0][t].id<<" ";
      //   }
      // }
    }
  auto e2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff2 = e2 - s;
  std::cerr << "Search Time diff2: " << diff2.count() << std::endl;
  for(unsigned i=0;i<query_num;i++)
  {
     new_res[i].clear();
    for(unsigned j=0;j<K;j++)
    {
      new_res[i].push_back(total_record[i][j].id);
    }
  }
  
  auto e = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> diff = e - s;
  std::cerr << "Search Time: " << diff.count() << std::endl;

  std::cout << "DistCount: " << totalCount << std::endl;
  save_result(argv[7], new_res);
  for(unsigned i=0;i<query_num;i++)
  {
     std::vector<unsigned>().swap(new_res[i]);
    
  }
  ///////////////////////////////////////////////////
  
  int *gt = NULL;
  int *gt_ = NULL;

  int p1,p2;
  int dim1,dim2;
  
  load_data(argv[5], gt,  p1,dim1); //groundtruth 
  load_data(argv[7], gt_, p2,dim2);  //result
 
 
  
  if(p1 != p2){std::cout<< "result file and groundtruth don't match,something wrong may hapeen in loaddata for result or groundtruth" <<std::endl; exit(-1);}
  
  int kNN = atoi(argv[8]);

  if(kNN > dim1){std::cout<< "result file not enough for k="<<kNN <<std::endl; exit(-1);}

  std::set<int> result;
  for(int i=0; i < p1; i++){
    result.clear();
    for(int j=0; j<kNN;j++){
      result.insert(gt[i*dim1+j]);
    }
    if (result.size()< (unsigned int) kNN)
      std::cout<< "query " << i <<" result index not unique!"<< std::endl;

  }
  
  int cnt=0;
  for(int i = 0; i < p1; i++){
    for(int j=0; j< kNN; j++){
      int k=0;
      
      for(; k<kNN; k++){
        if(gt[i*dim1+j]==gt_[i*dim2+k])break;
      }
      if(k==kNN)cnt++;
    }
  }
  std::cout<<cnt<<std::endl;
  std::cout<<(p1*kNN)<<std::endl;
  std::cout<<kNN <<"NN accuracy: "<<1-(float)cnt/(p1*kNN)<<std::endl;
    
  return 0;
}
