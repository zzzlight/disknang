#include <bits/stdc++.h>


using namespace std;

int main(){
    ifstream ifs("sift_base_4kw_correct.bin");
   // ofstream ofs("sift_base_4kw.fvecs");
    int n,dim;
   
    ifs.read((char*)&n,sizeof(int));
    ifs.read((char*)&dim,sizeof(int));
    char  dat;
   // ifs.seekg(-8L,ios::end);
   // int newn=(ifs.tellg())/dim*sizeof(unsigned char);
   // ifs.seekg(0,ios::beg);
    cout<<"n = "<<n<<endl;
   // cout<<"newn = "<<newn<<endl;
    cout<<"dim = "<<dim<<endl;
    cout<<"writing data..."<<endl;
    cout<<"unsigned:"<<endl;
    for(size_t i=0;i<1;i++){
	for(int j=0;j<dim;j++)
    	{
    		ifs.read((char*)&dat,sizeof(char));
    		float show=(float)dat; 
		if(i<10)cout<<show<<" ";
		}
        cout<<endl;
       
    }
    /*
    cout<<"char:"<<endl;
     ifs.seekg(0,ios::beg);
    ifs.read((char*)&n,sizeof(int));
    ifs.read((char*)&dim,sizeof(int));
     char tt;
    for(size_t i=0;i<20;i++){
    	for(int j=0;j<dim;j++)
    	{
    		ifs.read((char*)&tt,sizeof(char));
    		float show=tt; 
		if(i>10)cout<<show<<" ";
		}
        cout<<endl;
       
    }
    */
    cout<<"write ok."<<endl;
    return 0;
}
