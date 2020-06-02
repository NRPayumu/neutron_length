//root -l -q split_file.cc'("input_file",10000000)'

void split_file(string st, int NEVENT){
	stringstream ss;
	ss.str("");
	ss<<st.c_str()<<".dat";
	ifstream fin(ss.str().c_str());
	//fin>>isono;
	//cout<<isono<<endl;
	//for(int i=0;i<6;i++)file>>fno>>fU235[i]>>fU235e[i];

	int N = 1;
	ss.str("");
	ss<<st.c_str()<<"_split"<<NEVENT<<"_"<<N<<".dat";
	ofstream fout(ss.str().c_str());

	string line;

	for(int i=0;;i++){
		//if (!(fin>>line))break;							//One Word read
		if (!(getline(fin,line)))break;				//row read
		if(i%NEVENT==0 && i!=0){
			fout.close();
			N++;
			ss.str("");
			ss<<st.c_str()<<"_split"<<NEVENT<<"_"<<N<<".dat";
			ofstream fout(ss.str().c_str());
		}
		//e_plus_write << E_p << endl;
		//neutron_write<< E_n << endl;
		fout << line << endl;
		if(i%NEVENT==0)cout<<i<<endl;
		//if(i%1000==0)cout<<i<<endl;
	}
	//wfile.close();
}
