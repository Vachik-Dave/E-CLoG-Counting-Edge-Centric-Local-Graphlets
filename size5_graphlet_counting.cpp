/*
	----------- Algorithms for Counting 5-size graphlets -----------

	For more understanding check "Algorithm_implementation.pdf" file.
*/

#include "graph.cpp"

using namespace std;


// Efficient implementations for finding set intersection

// Returns intersection set given two sets and assuming node_ids are ordered
vector<NodeID_TYPE> intersection(vector<NodeID_TYPE> a, vector<NodeID_TYPE> b)
{
/*	// currently not neccessary to check this
	if(!is_sorted(a.begin(),a.end()))
		sort(a.begin(),a.end());
	if(!is_sorted(b.begin(),b.end()))
		sort(b.begin(),b.end());
//*/
	vector<NodeID_TYPE> ret;
	int i = 0;
	int j = 0;
	while(i < a.size() && j < b.size())
	{
		if(a[i] == b[j])
		{
			ret.push_back(a[i]);
			i++;
			j++;
		}
		else if(a[i] < b[j])
			i++;
		else
			j++;
	}
	return ret;
}

// Returns count of elements which are larger than a value from the intersection set.
vector<unsigned int> intersection(vector<NodeID_TYPE> a, vector<NodeID_TYPE> b, NodeID_TYPE val, int &ret_count)
{
	vector<NodeID_TYPE> ret;
	int i = 0;
	int j = 0;
	ret_count = 0;
	while(i < a.size() && j < b.size())
	{
		if(a[i] == b[j])
		{
			ret.push_back(a[i]);
			if(a[i] > val)
				ret_count ++;
			i++;
			j++;
		}
		else if(a[i] < b[j])
			i++;
		else
			j++;
	}
	return ret;
}

// Returns intersection set given 1 set and other set in format of CSR adjacency list
vector<NodeID_TYPE> intersection(NodeID_TYPE x, vector<NodeID_TYPE> b, NodeID_TYPE* &start_idx,NodeID_TYPE* &nei_size, NodeID_TYPE* & target_node_id)
{
/*	// currently not neccessary to check this
	if(!is_sorted(a.begin(),a.end()))
		sort(a.begin(),a.end());
	if(!is_sorted(b.begin(),b.end()))
		sort(b.begin(),b.end());
//*/
	vector<NodeID_TYPE> ret;
	int i = 0;
	int j = 0;
	while(i < nei_size[x] && j < b.size())
	{
		if(target_node_id[i+ start_idx[x]] == b[j])
		{
			ret.push_back(b[j]);
			i++;
			j++;
		}
		else if(target_node_id[i+ start_idx[x]] < b[j])
			i++;
		else
			j++;
	}
	return ret;
}

// Returns intersection set given two sets and assuming node_ids are ordered
vector<NodeID_TYPE> set_difference(vector<NodeID_TYPE> a, vector<NodeID_TYPE> b)
{
/*	// currently not neccessary to check this
	if(!is_sorted(a.begin(),a.end()))
		sort(a.begin(),a.end());
	if(!is_sorted(b.begin(),b.end()))
		sort(b.begin(),b.end());
//*/
	vector<NodeID_TYPE> ret;
	int i = 0;
	int j = 0;
	while(i < a.size() && j < b.size())
	{
		if(a[i] == b[j])
		{
			i++;
			j++;
		}
		else if(a[i] < b[j])
		{
			ret.push_back(a[i]);
			i++;
		}
		else
			j++;
	}
	while(i < a.size())
		ret.push_back(a[i++]);


	return ret;
}

// -------------------
// Implemetation of 5-size cycle and path graphlets g12 and g19
// Check "Algorithm_implementation.pdf".
// -------------------
void size5_path_cycle(NodeID_TYPE u, NodeID_TYPE v, graph & g, vector<NodeID_TYPE> & T,vector<NodeID_TYPE> & N_u, vector<NodeID_TYPE> & N_v, unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & T_key, unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & N_u_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & N_v_key, vector<Count_TYPE> & freq, vector<int> & status_map)
{
	Count_TYPE f12_i=0,f19_i=0;
	for(int idx = 0 ; idx < N_u.size();idx++)
	{
		Count_TYPE f12_uij=0,f19_ij=0;
		NodeID_TYPE i = N_u[idx];
		vector<NodeID_TYPE> N_v_bar = set_difference(N_v, N_v_key[i]);

		for(int idx1 =0; idx1 < g.nei_size[i]; idx1++)
		{
			NodeID_TYPE j = g.target_node_id[g.start_idx[i]+idx1];
			if(status_map[j] == 0)
			{
				vector<NodeID_TYPE> N_vij_bar = intersection(j,N_v_bar,g.start_idx,g.nei_size,g.target_node_id);
				f19_ij += N_vij_bar.size();
				f12_uij += (N_v_bar.size()-N_vij_bar.size());
			}

		}
		f12_i += f12_uij;
		f19_i += f19_ij;
	}

	for(int idx = 0 ; idx < N_v.size();idx++)
	{
		Count_TYPE f12_vij=0,f19_ij=0;
		NodeID_TYPE i = N_v[idx];
		vector<NodeID_TYPE> N_u_bar = set_difference(N_u, N_u_key[i]);

		for(int idx1 =0; idx1 < g.nei_size[i]; idx1++)
		{
			NodeID_TYPE j = g.target_node_id[g.start_idx[i]+idx1];
			if(status_map[j] == 0)
			{
				vector<NodeID_TYPE> N_uij_bar = intersection(j,N_u_bar,g.start_idx,g.nei_size,g.target_node_id);
				f12_vij += (N_u_bar.size()-N_uij_bar.size());
			}
		}
		f12_i += f12_vij;
	}

	freq[12] = f12_i;
	freq[19] = f19_i;

}

// -------------------
// Implemetation of Algorithms 1,2,3 and 6 of "Algorithm_implementation.pdf".
// -------------------
void algorithm1236(NodeID_TYPE u, NodeID_TYPE v,vector<NodeID_TYPE> & T,vector<NodeID_TYPE> & N_u,vector<NodeID_TYPE> & N_v,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & T_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & N_u_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & N_v_key, vector<Count_TYPE> & freq)
{
	//algo1
	Count_TYPE f45_i=0,f44_i=0,f39_i=0,f31_i=0;
	//algo6
	Count_TYPE f30_i=0,f21_i=0,f17_i=0,f15_i=0;
	Count_TYPE f27_i=0;
	//algo2
	Count_TYPE f43_i=0,f42_i=0,f40_i=0,f34_i=0;
	//algo3
	Count_TYPE f38_i=0,f33_i=0,f29_i=0,f22_i=0;
	Count_TYPE f41_i=0,f36_i=0,f35_i=0,f25_i=0;
	int error_counter = 0 ;
	for(int idx =0; idx < T.size();idx++)
	{
	// algo1
		NodeID_TYPE i = T[idx];
		Count_TYPE f45_ij=0,f44_ij=0,f39_ij=0,f31_uij=0,f31_vij=0;
		for(int idx1 = 0; idx1 < T_key[i].size(); idx1++)
		{
			NodeID_TYPE j = T_key[i][idx1];
			if(j < i)
			{
				vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
				f44_ij += T_key[j].size() - N_Tij.size() -1;
				continue;
			}

			// k in T
			int inter_size;
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j],j,inter_size);
			f45_ij += inter_size;
			f44_ij += T_key[j].size() - N_Tij.size() -1;
			f39_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f31_uij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f31_vij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f45_i = f45_i + f45_ij;
		f44_i = f44_i + f44_ij;
		f39_i = f39_i + f39_ij;
		f31_i = f31_i + f31_uij + f31_vij;

	// algo6
		Count_TYPE f30_ij=0,f21_uij=0,f21_vij=0;
		// j in T (N_Ti_bar)
		vector<NodeID_TYPE> temp_T;
		for(int k = 0 ;k < T.size();k++)
		{
			if(T[k] != i)
				temp_T.push_back(T[k]); 
		}
		vector<NodeID_TYPE> N_Ti_bar = set_difference(temp_T,T_key[i]);
		for(int idx1 = 0; idx1 < N_Ti_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_Ti_bar[idx1];
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f30_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size() - 2;
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f21_uij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f21_vij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f30_i = f30_i + (f30_ij/2);
		f21_i = f21_i + f21_uij + f21_vij;

		// j in N_u (N_ui_bar)
		vector<NodeID_TYPE> N_ui_bar = set_difference(N_u,N_u_key[i]);
		Count_TYPE f17_uij=0,f17_vij=0,f15_ij=0;
		Count_TYPE f27_ij=0;
		for(int idx1 = 0; idx1 < N_ui_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_ui_bar[idx1];
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f17_uij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size() -1;
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f27_ij += N_v_key[j].size() - N_vij.size();
			f15_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f17_i = f17_i + (f17_uij/2);
		f15_i = f15_i + f15_ij;
		f27_i = f27_i + f27_ij;

		// j in N_v (N_vi_bar)
		vector<NodeID_TYPE> N_vi_bar = set_difference(N_v,N_v_key[i]);
		for(int idx1 = 0; idx1 < N_vi_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_vi_bar[idx1];
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f17_vij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size() -1;
		}
		f17_i = f17_i + (f17_vij/2);

	//algo2   j in N_u
		Count_TYPE f43_ij_u=0,f42_ij_u=0,f40_ij_u=0,f34_ij_u=0;
	//algo3  j in N_u
		Count_TYPE f38_ij_u=0,f33_ij_u=0,f29_ij_u=0,f22_ij_u=0;
		Count_TYPE f41_ij_u=0,f36_ij_u=0,f35_ij_u=0,f25_ij_u=0;
		for(int idx1 = 0; idx1 < N_u_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_u_key[i][idx1];
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f43_ij_u += N_Tij.size();
			f42_ij_u += T_key[j].size() - N_Tij.size() -1;
			f40_ij_u += T_key[i].size() - N_Tij.size();
			f34_ij_u += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();

			// algo3
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f38_ij_u += N_uij.size();
			f33_ij_u += N_u_key[j].size() - N_uij.size();
			f29_ij_u += N_u_key[i].size() - N_uij.size() - 1;
			f22_ij_u += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f41_ij_u += N_vij.size();
			f36_ij_u += N_v_key[j].size() - N_vij.size();
			f35_ij_u += N_v_key[i].size() - N_vij.size();
			f25_ij_u += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();


		}
		//algo2
		f43_i = f43_i + f43_ij_u;
		f42_i = f42_i + f42_ij_u;
		f40_i = f40_i + f40_ij_u;
		f34_i = f34_i + f34_ij_u;


		//algo3
		f38_i = f38_i + f38_ij_u/2;
		f33_i = f33_i + f33_ij_u;
		f29_i = f29_i + f29_ij_u/2;
		f22_i = f22_i + f22_ij_u;
		f41_i = f41_i + f41_ij_u;
		f36_i = f36_i + f36_ij_u;
		f35_i = f35_i + f35_ij_u;
		f25_i = f25_i + f25_ij_u;

	//algo2   j in N_v
		Count_TYPE f43_ij_v=0,f42_ij_v=0,f40_ij_v=0,f34_ij_v=0;
	//algo3  j in N_v
		Count_TYPE f38_ij_v=0,f33_ij_v=0,f29_ij_v=0,f22_ij_v=0;
		Count_TYPE f36_ij_v=0,f25_ij_v=0;
		for(int idx1 = 0; idx1 < N_v_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_v_key[i][idx1];
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f43_ij_v += N_Tij.size();
			f42_ij_v += T_key[j].size() - N_Tij.size() -1;
			f40_ij_v += T_key[i].size() - N_Tij.size();
			f34_ij_v += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();

			// algo3
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f38_ij_v += N_vij.size();
			f33_ij_v += N_v_key[j].size() - N_vij.size();
			f29_ij_v += N_v_key[i].size() - N_vij.size() - 1;
			f22_ij_v += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f36_ij_v += N_u_key[j].size() - N_uij.size();
			f25_ij_v += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();

		}
		//algo2
		f43_i = f43_i + f43_ij_v;
		f42_i = f42_i + f42_ij_v;
		f40_i = f40_i + f40_ij_v;
		f34_i = f34_i + f34_ij_v;

		//algo3
		f38_i = f38_i + f38_ij_v/2;
		f33_i = f33_i + f33_ij_v;
		f29_i = f29_i + f29_ij_v/2;
		f22_i = f22_i + f22_ij_v;
		f36_i = f36_i + f36_ij_v;
		f25_i = f25_i + f25_ij_v;

	}

//----------------------------------
	//algo1
	freq[45] = f45_i;
	freq[44] = f44_i/2;
	freq[39] = f39_i;
	freq[31] = f31_i;
	//algo6
	freq[30] = f30_i/3;
	freq[21] = f21_i/2;
	freq[17] = f17_i;
	freq[15] = f15_i;
	freq[27] = f27_i;
	//algo2
	freq[43] = f43_i/2;
	freq[42] = f42_i/2;
	freq[40] = f40_i;
	freq[34] = f34_i;
	//algo3
	freq[38] = f38_i;
	freq[33] = f33_i;
	freq[29] = f29_i;
	freq[22] = f22_i;
	freq[41] = f41_i;
	freq[36] = f36_i;
	freq[35] = f35_i;
	freq[25] = f25_i;

}

// -------------------
// Implemetation of Algorithms 4 and 5 of "Algorithm_implementation.pdf".
// Modification of Algorithm 5 is renamed as Algorithm 7.
// -------------------
void algorithm457(NodeID_TYPE u, NodeID_TYPE v,vector<NodeID_TYPE> &T,vector<NodeID_TYPE> &N_u,vector<NodeID_TYPE> &N_v,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &T_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_u_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_v_key, vector<Count_TYPE> &freq)
{
	//algo4
	Count_TYPE f32_i=0,f23_i=0,f18_i=0,f37_i=0,f28_i=0,f16_i=0,f24_i=0;
	//algo5
	Count_TYPE f26_i=0,f20_i=0,f27_i=0;
	//algo7
	Count_TYPE f14_i=0,f13_i=0;
	int error_counter = 0 ;
//i in N_u
	for(int idx =0; idx < N_u.size();idx++)
	{
		NodeID_TYPE i = N_u[idx];
	// algo4  j in N_u
		Count_TYPE f32_ij=0,f23_ij=0,f18_ij=0,f37_ij=0,f28_ij=0,f16_ij=0,f24_ij=0;
		for(int idx1 = 0; idx1 < N_u_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_u_key[i][idx1];
			if(j < i)
			{
				vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
				f23_ij += N_u_key[j].size() - N_uij.size() - 1;
				vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
				f28_ij += N_v_key[j].size() - N_vij.size();
				continue;
			}
			// k in N_u
			int inter_size;
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j],j,inter_size);
			f32_ij += inter_size;
			f23_ij += N_u_key[j].size() - N_uij.size() - 1;
			f18_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f37_ij += N_vij.size();
			f28_ij += N_v_key[j].size() - N_vij.size();
			f16_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f24_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();
		}
		f32_i = f32_i + f32_ij;
		f23_i = f23_i + f23_ij;
		f18_i = f18_i + f18_ij;
		f37_i = f37_i + f37_ij;
		f28_i = f28_i + f28_ij;
		f16_i = f16_i + f16_ij;
		f24_i = f24_i + f24_ij;

	//algo7   j in N_u (N_ui_bar)
		Count_TYPE f26_ij=0,f20_ij=0;
		Count_TYPE f14_ij=0,f13_ij=0;
		vector<NodeID_TYPE> temp_N_u;
		for(int k = 0 ;k < N_u.size();k++)
		{
			if(N_u[k] != i)
				temp_N_u.push_back(N_u[k]); 
		}
		vector<NodeID_TYPE> N_ui_bar = set_difference(temp_N_u,N_u_key[i]);
		for(int idx1 = 0; idx1 < N_ui_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_ui_bar[idx1];
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f14_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size() - 2;
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f26_ij += N_vij.size();
			f20_ij += N_v_key[j].size() - N_vij.size();
			f13_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f14_i = f14_i + (f14_ij/2);
		f13_i = f13_i + f13_ij;
		f26_i = f26_i + f26_ij;
		f20_i = f20_i + f20_ij;

	}

//i in N_v
	for(int idx =0; idx < N_v.size();idx++)
	{
	// algo4  j in N_u
		NodeID_TYPE i = N_v[idx];
		Count_TYPE f32_ij=0,f23_ij=0,f18_ij=0,f37_ij=0,f28_ij=0,f16_ij=0,f24_ij=0;
		for(int idx1 = 0; idx1 < N_v_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_v_key[i][idx1];
			if(j < i)
			{
				vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
				f23_ij += N_v_key[j].size() - N_vij.size() - 1;
				vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
				f28_ij += N_u_key[j].size() - N_uij.size();
				continue;
			}
			// k in N_v
			int inter_size;
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j],j,inter_size);
			f32_ij += inter_size;
			f23_ij += N_v_key[j].size() - N_vij.size() - 1;
			f18_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f37_ij += N_uij.size();
			f28_ij += N_u_key[j].size() - N_uij.size();
			f16_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f24_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();
		}
		f32_i = f32_i + f32_ij;
		f23_i = f23_i + f23_ij;
		f18_i = f18_i + f18_ij;
		f37_i = f37_i + f37_ij;
		f28_i = f28_i + f28_ij;
		f16_i = f16_i + f16_ij;
		f24_i = f24_i + f24_ij;

	//algo7   j in N_v (N_vi_bar)
		Count_TYPE f26_ij=0,f20_ij=0;
		Count_TYPE f14_ij=0,f13_ij=0;
		vector<NodeID_TYPE> temp_N_v;
		for(int k = 0 ;k < N_v.size();k++)
		{
			if(N_v[k] != i)
				temp_N_v.push_back(N_v[k]); 
		}
		vector<NodeID_TYPE> N_vi_bar = set_difference(temp_N_v,N_v_key[i]);
		for(int idx1 = 0; idx1 < N_vi_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_vi_bar[idx1];
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f14_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size() - 2;
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f26_ij += N_uij.size();
			f20_ij += N_u_key[j].size() - N_uij.size();
			f13_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
		}
		f14_i = f14_i + (f14_ij/2);
		f13_i = f13_i + f13_ij;
		f26_i = f26_i + f26_ij;
		f20_i = f20_i + f20_ij;

	}
	//algo4
	freq[32] = f32_i;
	freq[23] = f23_i/2;
	freq[18] = f18_i;
	freq[37] = f37_i;
	freq[28] = f28_i;
	freq[16] = f16_i;
	freq[24] = f24_i;

	//algo5
	//algo7
	freq[14] = f14_i/3;
	freq[13] = f13_i/2;
	freq[26] = f26_i/2;
	freq[20] = f20_i;

}

/*
		***************************** End of Local Graphlet Counting Algorithms *****************************
*/					


//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

//					************ Unique **************

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

// 			For counting (structurally) unique graphlet (i.e. ignoring edge orbits) 

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

// It is a modification of the above algorithms to avoid duplicate counting of same structured graphlets.


void algorithm1236_unique(NodeID_TYPE u, NodeID_TYPE v,vector<NodeID_TYPE> & T,vector<NodeID_TYPE> & N_u,vector<NodeID_TYPE> & N_v,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & T_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & N_u_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > & N_v_key, vector<Count_TYPE> & freq)
{
	//algo1
	Count_TYPE f45_i=0,f44_i=0,f39_i=0,f31_i=0;
	//algo6
	Count_TYPE f30_i=0,f21_i=0,f17_i=0,f15_i=0;
	Count_TYPE f27_i=0;
	//algo2
//	Count_TYPE f43_i=0,f42_i=0,f40_i=0,f34_i=0;
	//algo3
//	Count_TYPE f38_i=0,f33_i=0,f29_i=0,f22_i=0;
	Count_TYPE f41_i=0,f36_i=0,f35_i=0,f25_i=0;
	int error_counter = 0 ;
	for(int idx =0; idx < T.size();idx++)
	{
	// algo1
		NodeID_TYPE i = T[idx];
		Count_TYPE f45_ij=0,f44_ij=0,f39_ij=0,f31_uij=0,f31_vij=0;
		for(int idx1 = 0; idx1 < T_key[i].size(); idx1++)
		{
			NodeID_TYPE j = T_key[i][idx1];
			if(j < i)
			{
				vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
				f44_ij += T_key[j].size() - N_Tij.size() -1;
				continue;
			}

			// k in T
			int inter_size;
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j],j,inter_size);
			f45_ij += inter_size;
			f44_ij += T_key[j].size() - N_Tij.size() -1;
			f39_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f31_uij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f31_vij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}

		f45_i = f45_i + f45_ij;
		f44_i = f44_i + f44_ij;
		f39_i = f39_i + f39_ij;
		f31_i = f31_i + f31_uij + f31_vij;

	// algo6
		Count_TYPE f30_ij=0,f21_uij=0,f21_vij=0;
		// j in T (N_Ti_bar)
		vector<NodeID_TYPE> temp_T;
		for(int k = 0 ;k < T.size();k++)
		{
			if(T[k] != i)
				temp_T.push_back(T[k]); 
		}
		vector<NodeID_TYPE> N_Ti_bar = set_difference(temp_T,T_key[i]);
		for(int idx1 = 0; idx1 < N_Ti_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_Ti_bar[idx1];
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f30_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size() - 2;
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f21_uij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f21_vij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f30_i = f30_i + (f30_ij/2);
		f21_i = f21_i + f21_uij + f21_vij;

		// j in N_u (N_ui_bar)
		vector<NodeID_TYPE> N_ui_bar = set_difference(N_u,N_u_key[i]);
		Count_TYPE f17_uij=0,f17_vij=0,f15_ij=0;
		Count_TYPE f27_ij=0;
		for(int idx1 = 0; idx1 < N_ui_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_ui_bar[idx1];
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f17_uij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size() -1;
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f27_ij += N_v_key[j].size() - N_vij.size();
			f15_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f17_i = f17_i + (f17_uij/2);
		f15_i = f15_i + f15_ij;
		f27_i = f27_i + f27_ij;

		// j in N_v (N_vi_bar)
		vector<NodeID_TYPE> N_vi_bar = set_difference(N_v,N_v_key[i]);
		for(int idx1 = 0; idx1 < N_vi_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_vi_bar[idx1];
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f17_vij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size() -1;
		}
		f17_i = f17_i + (f17_vij/2);

	//algo2   j in N_u
//		Count_TYPE f43_ij_u=0,f42_ij_u=0,f40_ij_u=0,f34_ij_u=0;
	//algo3  j in N_u
//		Count_TYPE f38_ij_u=0,f33_ij_u=0,f29_ij_u=0,f22_ij_u=0;
		Count_TYPE f41_ij_u=0,f36_ij_u=0,f35_ij_u=0,f25_ij_u=0;
		for(int idx1 = 0; idx1 < N_u_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_u_key[i][idx1];
			// algo 2 *code removed*
			// algo3
			// k in N_u
//			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
//			f38_ij_u += N_uij.size();
//			f33_ij_u += N_u_key[j].size() - N_uij.size();
//			f29_ij_u += N_u_key[i].size() - N_uij.size() - 1;
//			f22_ij_u += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f41_ij_u += N_vij.size();
			f36_ij_u += N_v_key[j].size() - N_vij.size();
			f35_ij_u += N_v_key[i].size() - N_vij.size();
			f25_ij_u += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();


		}

		f41_i = f41_i + f41_ij_u;
		f36_i = f36_i + f36_ij_u;
		f35_i = f35_i + f35_ij_u;
		f25_i = f25_i + f25_ij_u;

	//algo2   j in N_v
//		Count_TYPE f43_ij_v=0,f42_ij_v=0,f40_ij_v=0,f34_ij_v=0;
	//algo3  j in N_v
//		Count_TYPE f38_ij_v=0,f33_ij_v=0,f29_ij_v=0,f22_ij_v=0;
		Count_TYPE f36_ij_v=0,f25_ij_v=0;
		for(int idx1 = 0; idx1 < N_v_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_v_key[i][idx1];
			// algo 2 *code removed*
			// algo3
			// k in N_v
//			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
//			f38_ij_v += N_vij.size();
//			f33_ij_v += N_v_key[j].size() - N_vij.size();
//			f29_ij_v += N_v_key[i].size() - N_vij.size() - 1;
//			f22_ij_v += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f36_ij_v += N_u_key[j].size() - N_uij.size();
			f25_ij_v += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();

		}
		f36_i = f36_i + f36_ij_v;
		f25_i = f25_i + f25_ij_v;
	}

//----------------------------------
	//algo1
	freq[45] = f45_i;
	freq[44] = f44_i/2;
	freq[39] = f39_i;
	freq[31] = f31_i;
	//algo6
	freq[30] = f30_i/3;
	freq[21] = f21_i/2;
	freq[17] = f17_i;
	freq[15] = f15_i;
	freq[27] = f27_i;
	//algo2
//	freq[43] = f43_i/2;
//	freq[42] = f42_i/2;
//	freq[40] = f40_i;
//	freq[34] = f34_i;
	//algo3
//	freq[38] = f38_i;
//	freq[33] = f33_i;
//	freq[29] = f29_i;
//	freq[22] = f22_i;
	freq[41] = f41_i;
	freq[36] = f36_i;
	freq[35] = f35_i;
	freq[25] = f25_i;

}


void algorithm457_unique(NodeID_TYPE u, NodeID_TYPE v,vector<NodeID_TYPE> &T,vector<NodeID_TYPE> &N_u,vector<NodeID_TYPE> &N_v,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &T_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_u_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_v_key, vector<Count_TYPE> &freq)
{
	//algo4
//	Count_TYPE f32_i=0,f23_i=0,f18_i=0,f37_i=0,f28_i=0,f16_i=0,f24_i=0;
	Count_TYPE f16_i=0,f24_i=0;
	//algo5
	Count_TYPE f26_i=0,f20_i=0,f14_i=0,f13_i=0;
	int error_counter = 0 ;
//i in N_u
	for(int idx =0; idx < N_u.size();idx++)
	{
	// algo4  j in N_u
		NodeID_TYPE i = N_u[idx];
//		Count_TYPE f32_ij=0,f23_ij=0,f18_ij=0,f37_ij=0,f28_ij=0,f16_ij=0,f24_ij=0;
		Count_TYPE f16_ij=0,f24_ij=0;
		for(int idx1 = 0; idx1 < N_u_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_u_key[i][idx1];
			if(j < i)
			{
//				vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
//				f23_ij += N_u_key[j].size() - N_uij.size() - 1;
//				vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
//				f28_ij += N_v_key[j].size() - N_vij.size();
				continue;
			}
			// k in N_u
//			int inter_size;
//			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j],j,inter_size);
//			f32_ij += inter_size;
//			f23_ij += N_u_key[j].size() - N_uij.size() - 1;
//			f18_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();

			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
//			f37_ij += N_vij.size();
//			f28_ij += N_v_key[j].size() - N_vij.size();
			f16_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f24_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();
		}
		f16_i = f16_i + f16_ij;
		f24_i = f24_i + f24_ij;

	//algo5   j in N_u (N_ui_bar)
		Count_TYPE f26_ij=0,f20_ij=0;
		Count_TYPE f14_ij=0,f13_ij=0;
		vector<NodeID_TYPE> temp_N_u;
		for(int k = 0 ;k < N_u.size();k++)
		{
			if(N_u[k] != i)
				temp_N_u.push_back(N_u[k]); 
		}
		vector<NodeID_TYPE> N_ui_bar = set_difference(temp_N_u,N_u_key[i]);
		for(int idx1 = 0; idx1 < N_ui_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_ui_bar[idx1];
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f14_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size() - 2;
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f26_ij += N_vij.size();
			f20_ij += N_v_key[j].size() - N_vij.size();
			f13_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();
		}
		f14_i = f14_i + (f14_ij/2);
		f13_i = f13_i + f13_ij;
		f26_i = f26_i + f26_ij;
		f20_i = f20_i + f20_ij;
	}
//i in N_v
	for(int idx =0; idx < N_v.size();idx++)
	{
	// algo4  j in N_u
		NodeID_TYPE i = N_v[idx];
//		Count_TYPE f32_ij=0,f23_ij=0,f18_ij=0,f37_ij=0,f28_ij=0,f16_ij=0,f24_ij=0;
		Count_TYPE f16_ij=0,f24_ij=0;
		for(int idx1 = 0; idx1 < N_v_key[i].size(); idx1++)
		{
			NodeID_TYPE j = N_v_key[i][idx1];
			if(j < i)
			{
//				vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
//				f23_ij += N_v_key[j].size() - N_vij.size() - 1;
//				vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
//				f28_ij += N_u_key[j].size() - N_uij.size();
				continue;
			}
			// k in N_v
			int inter_size;
//			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j],j,inter_size);
//			f32_ij += inter_size;
//			f23_ij += N_v_key[j].size() - N_vij.size() - 1;
//			f18_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size();

			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
//			f37_ij += N_uij.size();
//			f28_ij += N_u_key[j].size() - N_uij.size();
			f16_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();

			// k in T
			vector<NodeID_TYPE> N_Tij = intersection(T_key[i],T_key[j]);
			f24_ij += T.size() - T_key[i].size() - T_key[j].size() + N_Tij.size();
		}
		f16_i = f16_i + f16_ij;
		f24_i = f24_i + f24_ij;

	//algo5   j in N_v (N_vi_bar)
		Count_TYPE f26_ij=0,f20_ij=0;
		Count_TYPE f14_ij=0,f13_ij=0;
		vector<NodeID_TYPE> temp_N_v;
		for(int k = 0 ;k < N_v.size();k++)
		{
			if(N_v[k] != i)
				temp_N_v.push_back(N_v[k]); 
		}
		vector<NodeID_TYPE> N_vi_bar = set_difference(temp_N_v,N_v_key[i]);
		for(int idx1 = 0; idx1 < N_vi_bar.size(); idx1++)
		{
			NodeID_TYPE j = N_vi_bar[idx1];
			// k in N_v
			vector<NodeID_TYPE> N_vij = intersection(N_v_key[i],N_v_key[j]);
			f14_ij += N_v.size() - N_v_key[i].size() - N_v_key[j].size() + N_vij.size() - 2;
			// k in N_u
			vector<NodeID_TYPE> N_uij = intersection(N_u_key[i],N_u_key[j]);
			f26_ij += N_uij.size();
			f20_ij += N_u_key[j].size() - N_uij.size();
			f13_ij += N_u.size() - N_u_key[i].size() - N_u_key[j].size() + N_uij.size();
		}
		f14_i = f14_i + (f14_ij/2);
		f13_i = f13_i + f13_ij;
		f26_i = f26_i + f26_ij;
		f20_i = f20_i + f20_ij;
	}
	freq[16] = f16_i;
	freq[24] = f24_i;

	//algo5
	freq[14] = f14_i/3;
	freq[13] = f13_i/2;
	freq[26] = f26_i/2;
	freq[20] = f20_i;

}


