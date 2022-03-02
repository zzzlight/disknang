#include <bits/stdc++.h>

using namespace std;

template<class T>
void writePOD(ofstream &ofs, T &dat){
    ofs.write((char*)&dat, sizeof(T));
}

template<class T>
void readPOD(ifstream &ifs, T &dat){
    ifs.read((char*)&dat, sizeof(T));
}

int main(){
    ifstream ifs("/home/zjlab/ANNS/yq/dataset/origin/bigann/bigann_base.bvecs",ios::binary);
    ofstream ofs("/home/zjlab/ANNS/zmq/Disk/sift_3kw.bin",ios::binary);
    int n,m;
    readPOD(ifs,n);
    ifs.seekg(0,ios::end);
    ios::pos_type ss=ifs.tellg();
    ifs.seekg(0,ios::beg);
   // m=(long)ss/(n+4);
    m=30000000;
    writePOD(ofs,m);
    writePOD(ofs,n);
    cout<<m<<' '<<n<<endl;
    
    for(int i=0;i<m;i++){
        int trash_int;
        readPOD(ifs,trash_int);
        // cout<<trash_int<<' ';
        for(int j=0;j<n;j++){
            char buffer;
            ifs.read((char*)&buffer,1);
			if(i==0) cout<<(float)buffer<<' ';
            writePOD(ofs, buffer);
        }
		// cout<<endl;
        if(i%1000000==0) cout<<i<<'/'<<m<<endl;
    }
    ifs.close();
    ofs.close();
    return 0;
}
