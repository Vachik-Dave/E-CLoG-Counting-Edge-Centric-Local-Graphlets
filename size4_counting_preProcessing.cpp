/*
	----------- Algorithm for Counting 4-size graphlets -----------
*/

#include "graph.cpp"

using namespace std;


inline int nChoose2(int n)
{
	if(n%2 == 0)
		return (n/2) * (n-1);
	else
		return n * ((n-1)/2); 
}




/*
edge (u,v)
NodeID_TYPE u, NodeID_TYPE v

graph in CSR formate
NodeID_TYPE* &start_idx,NodeID_TYPE* &nei_size, NodeID_TYPE* & target_node_id

RETURN Triangles (T), Neighbors of only u (N_u), Neighbors of only v (N_v)
vector<NodeID_TYPE> &T,vector<NodeID_TYPE> &N_u,vector<NodeID_TYPE> &N_v

RETURN (Neighbor interesection with T,N_u,N_v) for each node of u,v neoghbors.
unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &T_key,
unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_u_key,
unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_v_key
//*/

vector<Count_TYPE> get_freqency_vector(NodeID_TYPE u, NodeID_TYPE v,graph & g,vector<NodeID_TYPE> &T,vector<NodeID_TYPE> &N_u,vector<NodeID_TYPE> &N_v,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &T_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_u_key,unordered_map<NodeID_TYPE,vector<NodeID_TYPE> > &N_v_key,vector<int> & status_map)
{
	NodeID_TYPE w,r;

//	int* freq = new int[graphlet_count]();				// allocate memory and assigne all values to 0
	vector<Count_TYPE> freq (graphlet_count);				// allocate memory and assigne all values to 0

	status_map[u] = -1;
	status_map[v] = -2;

//	cout << u << " Neighbors: " ;
	for(int i =0; i < g.nei_size[u]; i++)
	{
		w = g.target_node_id[g.start_idx[u]+i];
//		cout << w << ",";
		if(w == v)
			continue;
		N_u.push_back(w);
		status_map[w] = 1;
	}
//	cout <<endl;
//	cout << v << " Neighbors: " ;

	for(int i =0; i < g.nei_size[v]; i++)
	{
		w = g.target_node_id[g.start_idx[v]+i];
//		cout << w << ",";
		if(w == u)
			continue;
		if(status_map[w] == 1)
		{
			vector<NodeID_TYPE>::iterator it1 = find (N_u.begin(), N_u.end(), w);
			N_u.erase(it1);
			T.push_back(w);
			status_map[w] = 3;
		}
		else
		{
			N_v.push_back(w);
			status_map[w] = 2;
		}
	}

	freq[1] = T.size();
	freq[0] = N_u.size() + N_v.size();

// ----- all 4-size graphlets are made of nodes (u,v,w,r)

	// 3rd node (w) from set T
	for(int i = 0 ; i < T.size();i++)
	{
		vector<NodeID_TYPE> T_w,N_u_w,N_v_w;
		w = T[i];
		for(int j =0; j < g.nei_size[w]; j++)
		{
			// 4th node r is neighbor of w
			r = g.target_node_id[g.start_idx[w]+j];

			if(r == u || r == v)
				continue;

			// r is in set T
			if(status_map[r] == 3)
			{
				T_w.push_back(r);
				freq[11]++;
			}
			// r is in set Nu
			else if(status_map[r] == 1)
				N_u_w.push_back(r);
			// r is in set Nv
			else if(status_map[r] == 2)
				N_v_w.push_back(r);
			// r is in set T, but the pair of nodes r and w already contributed for f11.
			else if(status_map[r] == 4)
				T_w.push_back(r);
			// r is neither u's neighbor node v's neighbor (non-local graphlet)
			else if(status_map[r] == 0)
				freq[7]++;
			else
				printf("status_map Error!!!\n");
		}
		status_map[w] = 4;
		// saving for future use (for 5-size graphlet counting)
		T_key[w] = T_w;
		N_u_key[w] = N_u_w;
		N_v_key[w] = N_v_w;
	}

// ---- Just for debugging
//	vector<unsigned long> N_u_counter (N,0);
//	Count_TYPE N_u_new = 0;

	// 3rd node (w) from set Nu
	for(int i = 0 ; i < N_u.size();i++)
	{
		vector<NodeID_TYPE> T_w,N_u_w,N_v_w;
		w = N_u[i];
		for(int j =0; j < g.nei_size[w]; j++)
		{
			// 4th node r is neighbor of w
			r = g.target_node_id[g.start_idx[w]+j];
			if(r == u)
				continue;
			// r is in set Nu
			if(status_map[r] == 1)
			{
				N_u_w.push_back(r);
				freq[6]++;
			}
			// r is in set Nv
			else if(status_map[r] == 2)
			{
				N_v_w.push_back(r);
				freq[5]++;
			}
			// r is in set T
			else if(status_map[r] == 4)
			{
				T_w.push_back(r);
				freq[9]++;
			}
			// r is in set Nu, but the pair of nodes r and w already contributed for f6.
			else if(status_map[r] == 5)
				N_u_w.push_back(r);
			// r is neither u's neighbor node v's neighbor (non-local graphlet)
			else if(status_map[r] == 0)
			{
//				status_map[r] = 7;
//				N_u_counter[r] = 1;
//				N_u_new++;
				freq[3]++;
			}
/* --- Debugging...
			else if(status_map[r] == 7)
			{
				N_u_counter[r]+=1;
				N_u_new++;
				freq[10]++;
			}
//			else
//				printf("status_map Error!!!\n");
//*/
		}
		status_map[w] = 5;
		// saving for future use (for 5-size graphlet counting)
		T_key[w] = T_w;
		N_u_key[w] = N_u_w;
		N_v_key[w] = N_v_w;
	}

// ---- Just for debugging 
//	Count_TYPE N_v_new = 0;
//	Count_TYPE N_u_minus = 0;
//	Count_TYPE N_v_minus = 0;
//	Count_TYPE cycle_5 = 0;

	// 3rd node (w) from set Nu
	for(int i = 0 ; i < N_v.size();i++)
	{
		vector<NodeID_TYPE> T_w,N_u_w,N_v_w;
		w = N_v[i];
		for(int j =0; j < g.nei_size[w]; j++)
		{
			// 4th node r is neighbor of w
			r = g.target_node_id[g.start_idx[w]+j];
			if(r == v)
				continue;
			// r is in set Nv
			if(status_map[r] == 2)
			{
				N_v_w.push_back(r);
				freq[6]++;
			}
			// r is in set T
			else if(status_map[r] == 4)
			{
				T_w.push_back(r);
				freq[9]++;
			}
			// r is in set Nu, not counting f5 because for pair (w,r) w from Nu and r from Nv is same as w from Nv and r from Nu.
			else if(status_map[r] == 5)
				N_u_w.push_back(r);
			// r is in set Nv, but the pair of nodes r and w already contributed for f6.
			else if(status_map[r] == 6)
				N_v_w.push_back(r);
			// r is neither u's neighbor node v's neighbor (non-local graphlet)
			else if(status_map[r] == 0)
			{
				freq[3]++;
//				N_v_new++;
			}
/* --- Debugging...
			else if(status_map[r] == 7)
			{
				N_v_new++;
				freq[10]++;
				cycle_5 += N_u_counter[r];
			}
//*/
/*

			else if(status_map[r] == 0)
			{
				status_map[r] = 8;
				freq[10]++;
				N_v_new++;
			}
			else if(status_map[r] == 8)
			{
				N_v_new++;
				freq[10]++;
			}
			else if(status_map[r] == 7)
			{
				status_map[r] = 9;
				N_v_new++;
				freq[10]++;
				N_u_minus++;
				N_v_minus += N_u_counter[r];
				cycle_5 += N_u_counter[r];
			}
			else if(status_map[r] == 9)
			{
				N_v_new++;
				freq[10]++;
				N_u_minus++;
				N_v_minus += N_u_counter[r];
				cycle_5 += N_u_counter[r];
			}
//*/
//			else
//				printf("status_map Error!!!\n");
		}
		status_map[w] = 6;
		// saving for future use (for 5-size graphlet counting)
		T_key[w] = T_w;
		N_u_key[w] = N_u_w;
		N_v_key[w] = N_v_w;
	}

	// Calculate other frequencies using combinatorial approach.
	freq[8] = freq[0]*freq[1] - freq[9];
	freq[10] = nChoose2(freq[1]) - freq[11];
	if(nChoose2(freq[1]) < freq[11])
		freq[10] = 0;
	freq[2] = (N_u.size() * N_v.size()) - freq[5];
	
	freq[4] = nChoose2(N_u.size()) + nChoose2(N_v.size()) - freq[6];
	if( (nChoose2(N_u.size()) + nChoose2(N_v.size())) < freq[6])
		freq[4] = 0;
	
	return freq;
}

