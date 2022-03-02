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

    srand(unsigned(time(0)));
    std::cerr << "Using time Seed " << std::endl;
  

  
 
  std::cerr << "Query Path: " << argv[2] << std::endl;

  unsigned query_num, query_dim;
  float* query_load = nullptr;
  efanna2e::load_data(argv[2], query_load, query_num, query_dim);
  //query_load = efanna2e::data_align(query_load, query_num, query_dim);

  //assert(dim == query_dim);

  std::cerr << "Data Path: " << argv[1] << std::endl;

  unsigned points_num, dim;
  float* data_load = nullptr;
  efanna2e::new_load_data(argv[1], data_load, points_num, dim);
  //data_load = efanna2e::data_align(data_load, points_num, dim);

  efanna2e::IndexRandom init_index(dim, points_num);
  efanna2e::IndexGraph index(dim, points_num, efanna2e::FAST_L2,
                           (efanna2e::Index*)(&init_index));

  index.Load(argv[3]);
  index.OptimizeGraph(data_load);

  unsigned L = (unsigned)atoi(argv[4]);
  unsigned K = (unsigned)atoi(argv[5]);

  std::cerr << "L = " << L << ", ";
  std::cerr << "K = " << K << std::endl;

  efanna2e::Parameters paras;
  paras.Set<unsigned>("L_search", L);

    std::cerr << "DNG Path: " << argv[3] << std::endl;
    std::cerr << "Result Path: " << argv[6] << std::endl;

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
  auto s = std::chrono::high_resolution_clock::now();
  
    unsigned offset=0;
    for (unsigned i = 0; i < query_num; i++) {
    index.MySearchWithOptGraph(query_load + i * dim, K, paras, res[i].data(),offset);
    }
    
    for(unsigned x=0;x<query_num;x++)
    {
      for(unsigned y=0;y<K;y++)
      {
        total_record[x].push_back(res[x][y]);
      }
      
    }
  

  offset+=25000000;
  float* data_load2 = nullptr;
  efanna2e::new_load_data(argv[9], data_load2, points_num, dim);
  index.Load(argv[10]);
  index.OptimizeGraph(data_load2);

  for(unsigned i=0;i<query_num;i++) res[i].clear();
  for (unsigned i = 0; i < query_num; i++) {
    index.MySearchWithOptGraph(query_load + i * dim, K, paras, res[i].data(),offset);
  }

  for(unsigned x=0;x<query_num;x++)
    {
      for(unsigned y=0;y<K;y++)
      {
        total_record[x].push_back(res[x][y]);
      }
      
    }

  
  offset+=25000000;
  float* data_load1 = nullptr;
  efanna2e::new_load_data(argv[11], data_load1, points_num, dim);
  index.Load(argv[12]);
  index.OptimizeGraph(data_load1);

  for(unsigned i=0;i<query_num;i++) res[i].clear();
  for (unsigned i = 0; i < query_num; i++) {
    index.MySearchWithOptGraph(query_load + i * dim, K, paras, res[i].data(),offset);
  }

  for(unsigned x=0;x<query_num;x++)
    {
      for(unsigned y=0;y<K;y++)
      {
        total_record[x].push_back(res[x][y]);
      }
      
    }
    
   offset+=25000000;
  float* data_load3 = nullptr;
  efanna2e::new_load_data(argv[13], data_load3, points_num, dim);
  index.Load(argv[14]);
  index.OptimizeGraph(data_load3);

  for(unsigned i=0;i<query_num;i++) res[i].clear();
  for (unsigned i = 0; i < query_num; i++) {
    index.MySearchWithOptGraph(query_load + i * dim, K, paras, res[i].data(),offset);
  }

  for(unsigned x=0;x<query_num;x++)
    {
      for(unsigned y=0;y<K;y++)
      {
        total_record[x].push_back(res[x][y]);
      }
      
    }



    for(unsigned x=0;x<query_num;x++)
    {
      std::sort(total_record[x].begin(),total_record[x].end(),cmp);
      if(x==0) 
      {
        std::cout<<"record is:"<<std::endl;
        for(unsigned t=0;t<total_record[0].size();t++)
        {
          std::cout<<total_record[0][t].id<<" ";
        }
      }
    }



  //}

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

  std::cout << L << "\t" << diff.count() << "\t" << "\n";
  std::cout << "DistCount: " << index.GetDistCount() << std::endl;
  save_result(argv[6], new_res);
  ///////////////////////////////////////////////////
  
  int *gt = NULL;
  int *gt_ = NULL;

  int p1,p2;
  int dim1,dim2;
  //std::cout<<argv[6]<<" "<<argv[7]<<std::endl;
  
  load_data(argv[7], gt,  p1,dim1); //groundtruth 
  load_data(argv[6], gt_, p2,dim2);  //result
 
 
  
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
    
  /////////////////////////////////////////////////
  return 0;
}
