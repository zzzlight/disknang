#include <iostream>
#include <fstream>
#include <set>
#include<vector>

using namespace std;

void load_data(char* filename, int*& data, int& num,int& dim){// load data with sift10K pattern
  ifstream in(filename, ios::binary);
  if(!in.is_open()){cout<<"open file error"<<endl;return;}
  in.read((char*)&dim,4);
  in.seekg(0,ios::end);
  ios::pos_type ss = in.tellg();
  int fsize = (int)ss;
  num = fsize / (dim+1) / 4;
  data = new int[num*dim];

  in.seekg(0,ios::beg);
  for(int i = 0; i < num; i++){
    in.seekg(4,ios::cur);
    in.read((char*)(data+i*dim),dim*4);
  }
  in.close();
}

int main(int argc, char** argv){

 int *gt = NULL;
 // int *gt_ = NULL;
  int *gt1 = NULL;
  int *gt2 = NULL;
  int *gt3 = NULL;
  int p1,p2,p3,p4;
  int dim1,dim2,dim3,dim4;
  //std::cout<<argv[6]<<" "<<argv[7]<<std::endl;
  load_data(argv[1], gt,  p1,dim1); //groundtruth 
  load_data(argv[3], gt1, p2,dim2);  //result
  load_data(argv[4], gt2, p3,dim3);  //result
  load_data(argv[5], gt3, p4,dim4);  //result
  // p2=query_num;
  // dim2=K;
  cout<<"dim2:"<<dim2<<" "<<"dim3:"<<dim3<<endl;
  cout<<"p1"<<p1<<endl;
  //if(p1 != p2+p3+p4){std::cout<< "result file and groundtruth don't match,something wrong may hapeen in loaddata for result or groundtruth" <<std::endl; exit(-1);}
  
  int kNN = atoi(argv[2]);

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
  /*
  merge
  */
    int GK=500;
    vector<vector<int>> gt_;
    for(int i=0;i<10000;i++)
    {
        vector<int> temp;
        for(int j=0;j<GK;j++)
        {
            temp.push_back(gt1[i*GK+j]);
        }
        for(int j=0;j<GK;j++)
        {
            temp.push_back(gt2[i*GK+j]);
        }
        for(int j=0;j<GK;j++)
        {
            temp.push_back(gt3[i*GK+j]);
        }
        gt_.push_back(temp);
    }


  int cnt=0;
  for(int i = 0; i < p1; i++){
    for(int j=0; j< kNN; j++){
      int k=0;
      
      for(; k<GK*3; k++){
        if(gt[i*dim1+j]==gt_[i][k])break;
      }
      if(k==3*GK)cnt++;
      
      //  for(; k<100; k++){
      //   if(gt[i*dim1+j]==gt_[i*dim2+k])break;
      // }
      // if(k<10000)cnt++;
    }
  }
  std::cout<<cnt<<std::endl;
  std::cout<<(p1*kNN)<<std::endl;
  std::cout<<kNN <<"NN accuracy: "<<1-(float)cnt/(p1*kNN)<<std::endl;
    return 0;
}