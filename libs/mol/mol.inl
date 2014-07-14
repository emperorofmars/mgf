/*
**	Author:		Martin Schwarz
**	Name:		mol.inl
**	Project:	mol - Mars OBJ Loader
**	Compile:	-std=c++11 // include in other project
**	License:	WTFPL
*/
//namespace mol{

//###############################################################  mesh class

//###############################################################  constructor
template<typename T> mesh<T>::mesh(){
}

template<typename T> mesh<T>::mesh(std::string file){
	if(!load_file(file)){
		vert.clear();
		norm.clear();
		vert_index.clear();
		norm_index.clear();
	}
}

template<typename T> mesh<T>::~mesh(){
	vert.clear();
	norm.clear();
	vert_index.clear();
	norm_index.clear();
}

//###############################################################  load from file
template<typename T> bool mesh<T>::load_file(std::string file){
	std::ifstream in(file);
	if(!in.is_open()){
		std::cerr << "failed to load file: " << file << std::endl;
		return false;
	}
	vn = 0;
	std::string line;
	std::vector<mvm::vecN<T, 4>> vert_tmp;
	std::vector<mvm::vecN<T, 3>> norm_tmp;
	while(getline(in, line)){
		if(line.substr(0, 2) == "v "){
			std::istringstream s(line.substr(2));
			mvm::vecN<T, 4> t;
			s >> t.data[0] >> t.data[1] >> t.data[2];
			t.data[3] = 1;
			vert_tmp.push_back(t);
			//std::cerr << vert_tmp[vert_tmp.size() - 1] << " " << vert_tmp.size() << std::endl;
		}
		else if(line.substr(0, 2) == "vn"){
			vn = 1;
			std::istringstream s(line.substr(2));
			mvm::vecN<T, 3> t;
			s >> t.data[0] >> t.data[1] >> t.data[2];
			norm_tmp.push_back(t);
			//std::cerr << norm_tmp[norm_tmp.size() - 1] << " " << norm_tmp.size() << std::endl;
		}
		else if(line.substr(0, 2) == "f "){
			std::istringstream s(line.substr(2));
			if(vn){
				unsigned int x;
				for(unsigned int i = 0; i < 3; i++){
					s >> x;
					x--;
					vert_index.push_back(x);
					//std::cerr << x << " ";
					s.seekg((int)s.tellg() + 2);
					s >> x;
					x--;
					norm_index.push_back(x);
					//std::cerr << x << std::endl;
				}
			}
			else{
				unsigned int x;
				for(unsigned int i = 0; i < 3; i++){
					s >> x;
					x--;
					vert_index.push_back(x);
					//std::cerr << x << std::endl;
				}
			}
			/*std::cerr << vert_index[vert_index.size() - 3] << " " << vert_index.size() - 3 << " // ";
			std::cerr << vert_index[vert_index.size() - 2] << " " << vert_index.size() - 2 << " // ";
			std::cerr << vert_index[vert_index.size() - 1] << " " << vert_index.size() - 1 << std::endl;*/
		}
		else if(line[0] == '#'){
			//ignore
		}
		else{
			//implement later
		}
	}
	if(vn){
		std::cerr << "normals enabled" << std::endl;
	}
	//norm.resize(tmp.size());
	for(unsigned int i = 0; i < vert_index.size(); i += 3){
		vert.push_back(vert_tmp[vert_index[i]]);
		vert.push_back(vert_tmp[vert_index[i + 1]]);
		vert.push_back(vert_tmp[vert_index[i + 2]]);
		if(vn){
			norm.push_back(norm_tmp[norm_index[i]]);
			norm.push_back(norm_tmp[norm_index[i + 1]]);
			norm.push_back(norm_tmp[norm_index[i + 2]]);
		}
		//std::cerr << vert_index[i] << " " << i << std::endl;
		//std::cerr << vert[i] << " " << vert[i + 1] << " " << vert[i + 2] << std::endl;
	}
	//std::cerr << elm.size() << std::endl;

	std::cerr << "loaded successfully: " << file << std::endl;
	return true;
}

//###############################################################  get array
template<typename T> mvm::vecN<T, 4>* mesh<T>::get_vertices(){
	return &vert[0];
}

template<typename T> unsigned int mesh<T>::get_size(){
	return vert.size() * sizeof(mvm::vecN<T, 4>);
}

template<typename T> unsigned int mesh<T>::get_num(){
	return vert.size();
}










