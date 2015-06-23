#include <bits/stdc++.h>
#include <boost/algorithm/string/replace.hpp>
using namespace std;
#define FREEBASE_THRESHOLD 100.0
const long long max_size = 200;         // max length of strings
const long long N = 5;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries
float *M;
long long words,size;
char *vocab;
int dp[100][100];
void print(string str)
{
	string s1,s2;
	stringstream ss(str);
	while (ss>>s1>>s2) {
		cout<<s1<<" ";
	}
	cout<<endl;
}
string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}
string lower(string s)
{
	string ret = s;
	for(int i = 0; i < ret.length(); i++) {
		if(ret[i] >='A' && ret[i] <= 'Z') {
			ret[i] += 'a'-'A';
		}
	}
	return ret;
}
vector<string> getSuggestions(const char str[])
{
	int xyz;
	//scanf("%d", &xyz);
	//cout<<s<<endl;
    //printf("%s", str);
    int n = strlen(str);
	vector<string> ret;
  	char bestw[N][100];
  	char st[100][max_size];
  	float dist, len, bestd[N], vec[max_size];
  	long long a, b, c, d, cn, bi[100];
  	int i;
    //printf("%s %d", str, n);
  	//scanf("%d", &xyz);
    //printf("%s %d", str, n);
  	for (a = 0; a < N; a++) bestd[a] = 0;
    for (a = 0; a < N; a++) bestw[a][0] = 0;
    //cout<<s<<s.size()<<endl;
    //scanf("%d", &xyz);
	for(i = 0; i < n; i++) {
		st[0][i] = str[i];
		//cout<<str[i];
	}
	//scanf("%d", &xyz);
	st[0][i] = 0;
    cn = 0;
    b = 0;
    c = 0;
    //scanf("%d", &xyz);
    cn++;
    for (a = 0; a < cn; a++) {
      for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], st[a])) break;
      if (b == words) b = -1;
      bi[a] = b;
      //printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
      if (b == -1) {
        //printf("Out of dictionary word!\n");
        return ret;
      }
    }
    //scanf("%d", &xyz);
    //printf("\n                                              Word       Cosine distance\n------------------------------------------------------------------------\n");
    //scanf("%d", &xyz);
    for (a = 0; a < size; a++) vec[a] = 0;
    for (b = 0; b < cn; b++) {
      if (bi[b] == -1) continue;
      for (a = 0; a < size; a++) vec[a] += M[a + bi[b] * size];
    }
    len = 0;
    for (a = 0; a < size; a++) len += vec[a] * vec[a];
    //cout<<len<<endl;
    len = sqrt(len);
    //scanf("%d", &xyz);
    for (a = 0; a < size; a++) vec[a] /= len;
    for (a = 0; a < N; a++) bestd[a] = -1;
    for (a = 0; a < N; a++) bestw[a][0] = 0;
    for (c = 0; c < words; c++) {
      a = 0;
      for (b = 0; b < cn; b++) if (bi[b] == c) a = 1;
      if (a == 1) continue;
      dist = 0;
      for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
      for (a = 0; a < N; a++) {
        if (dist > bestd[a]) {
          for (d = N - 1; d > a; d--) {
            bestd[d] = bestd[d - 1];
            strcpy(bestw[d], bestw[d - 1]);
          }
          bestd[a] = dist;
          strcpy(bestw[a], &vocab[c * max_w]);
          break;
        }
      }
    }
    for (a = 0; a < N; a++) {
    	//printf("%s\n", bestw[a]);
    	string temp="";
    	for(i = 0; bestw[a][i]; i++) {
    		temp += bestw[a][i];
    	}
    	ret.push_back(temp);
    }
    return ret;
 }
float parse(string s)
{
	return (float)atof(s.c_str());
}
vector<string> getFreebaseSuggestions(string s)
{
	vector<string> ret;
	//ret.push_back("picture");
	string query = "python api.py ";
	query += s;
	char q[100];
	int i;
	for(i = 0; i < query.length(); i++) {
		q[i] = query[i];
	}
	int count = 10;
	q[i] = 0;
	string temp = exec(q);
	//cout<<temp<<endl;
	stringstream ss(temp);
	string score ;
	while(getline(ss, query)) {
		count++;
		//ss>>score;
		//cout<<query<<endl;
		//getline(ss, score);
		ret.push_back(query);
	}
	return ret;
}
void preprocess()
{
	FILE *f;
	f = fopen("vectors-wiki.bin", "rb");
  	if (f == NULL) {
    	printf("Input file not found\n");
    	return ;
  	}
  	long long a, b;
  	float len;
  	fscanf(f, "%lld", &words);
  	fscanf(f, "%lld", &size);
  	vocab = (char *)malloc((long long)words * max_w * sizeof(char));
  	M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  	if (M == NULL) {
    	printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    	return ;
  	}
  	for (b = 0; b < words; b++) {
    	a = 0;
    	while (1) {
      		vocab[b * max_w + a] = fgetc(f);
      		if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
      		if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
    	}
    	vocab[b * max_w + a] = 0;
    	for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
    	len = 0;
    	for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    	len = sqrt(len);
    	for (a = 0; a < size; a++) M[a + b * size] /= len;
  	}
  	fclose(f);
}
bool isNLTKnoun(string s)
{
	string temp = "python NLTKnoun.py ";
	char quer[1000];
	temp += s;
	int i;
	for(i = 0; i < temp.length(); i++) quer[i] = temp[i];
	quer[i] = 0;
	temp = exec(quer);
	if (temp.length() < 1) return 0;
	return temp[0]-'0';
}
int isFreebaseNoun(vector<string> now, int s, int e)
{
	string str="";
	for(int i = s; i < e; i++) {
		str += now[i];
		str += "_";
	}
	str += now[e];
	string temp = "python isFreebaseNoun.py ";
	temp += str;
	char quer[1000];
	int i;
	for(i = 0; i < temp.length(); i++) quer[i] = temp[i];
	quer[i] = 0;
	temp = exec(quer);
	if (temp.length() < 1) return 0;
	return temp[0]-'0';
}
int isFreebaseNoun(string str)
{
	string temp = "python isFreebaseNoun.py ";
	temp += str;
	char quer[1000];
	int i;
	for(i = 0; i < temp.length(); i++) quer[i] = temp[i];
	quer[i] = 0;
	temp = exec(quer);
	if (temp.length() < 1) return 0;
	return temp[0]-'0';
}
int splitter(vector<string> now, int s, int e)
{
	if(s>=e) return dp[s][e] = 0;
	if(isFreebaseNoun(now, s, e)) return dp[s][e] = 0;
	if(dp[s][e] != -1) return dp[s][e];
	int ans = 9999;
	for(int i = s; i < e; i++) {
		ans = min(ans, 1 + splitter(now, s,i)+splitter(now, i+1,e));
	}
	return dp[s][e] = ans;
}
string rebuild_sol(vector<string> now, int s, int e)
{
	if(s>e)
		return "";

	string str;
	if (s==e) {
		return now[s];
	}
	if(isFreebaseNoun(now, s, e)) {
		str = "";
		for(int i = s; i < e; i++) {
			str += now[i];
			str += "_";
		}
		str += now[e];
		return str;
	}
	str = "";
	for(int i = s; i < e; i++) {
		if (dp[s][e] == -1 || dp[s][i] == -1 || dp[i+1][e] == -1 ) continue;
		if (dp[s][e] == 1 + dp[s][i] + dp[i+1][e]){
			str = rebuild_sol(now, s, i);
			str += " ";
			str += rebuild_sol(now, i+1, e);
			return str;
		}
	}
	return str;
}
string convert(string s)
{
	vector<string> now;
	string temp;
	stringstream ss(s);
	while(ss>>temp) {
		now.push_back(temp);
	}
	memset(dp, -1, sizeof dp);
	int ans = splitter(now, 0, (int)now.size() - 1);
	temp = "";
	temp = rebuild_sol(now, 0, (int)now.size() - 1);
	//cout<<"the new string is "<<temp<<endl;
	return temp;
}
string clean(string a)
{
	for(int i = 0; i < a.length(); i++) {
		if (a[i] >= 'A' && a[i] <= 'Z') a[i] += 'a' - 'A';
		if (a[i] == ' ') a[i] = '_';
	}
	return a;
}
int sameFreebaseCategory(string a, string b)
{
	a = clean(a);
	b = clean(b);
	string temp, res1="", res2="";
	temp = "python api2.py ";
	temp += a;
	char quer[1000];
	int i;
	for(i = 0; i < temp.length(); i++) quer[i] = temp[i];
	quer[i] = 0;
	res1 = exec(quer);

	temp = "python api2.py ";
	temp += b;
	for(i = 0; i < temp.length(); i++) quer[i] = temp[i];
	quer[i] = 0;
	res2 = exec(quer);

	return res1 == res2 && res1.length() > 0 && res2.length() > 0;
}
int main(int argc, char const *argv[])
{
	
  	preprocess();
  	
	string str, s, tag;
	int count = 1;
	vector<string> vs;
	fstream fs("taxoTagscleaned.txt");
	freopen("taxoSUGGbothwikiwithNOUNpart2.csv", "w", stdout);
	while(getline(fs, str)) {
		//stringstream ss(str);
		stringstream ss2(str);
		string newstr = str;
		string finstr = "";
		while (ss2>>s>>tag) {
			finstr += s;
			finstr += " ";
		}
		//cout<<"Orignial Query: "<<finstr<<endl;
		string nounstr = convert(finstr);
		//cout<<"Modified Query: "<<nounstr<<endl;
		stringstream ss(nounstr);
		//cout<<"Suggestions: "<<endl;
		while (ss>>s) {
			//if (tag.find("nn") != string::npos) {
			//if (!isNLTKnoun(s)) continue; //Dictionary not working correctly !
			if (isFreebaseNoun(s)) {
				vs = getSuggestions(s.c_str());
				for(int i = 0; i < vs.size(); i++) {
					newstr = nounstr;
					if (!sameFreebaseCategory(s, vs[i])) continue;
					boost::replace_all(newstr, s, vs[i]);
					//print(newstr);
					//cout<<"Replacing "<<s<<" with "<<vs[i]<<" : "<<newstr<<endl;
					cout<<finstr<<","<<nounstr<<","<<s<<","<<vs[i]<<","<<newstr<<endl;
				}
			}
			vs = getFreebaseSuggestions(s);
			for(int i = 0; i < vs.size(); i++) {
				newstr = nounstr;
				if (!sameFreebaseCategory(s, vs[i])) continue;
				boost::replace_all(newstr, s, vs[i]);
				//print(newstr);
				//cout<<"Replacing "<<s<<" with "<<vs[i]<<" : "<<newstr<<endl;
				cout<<finstr<<","<<nounstr<<","<<s<<","<<vs[i]<<","<<newstr<<endl;
			}	
		}
		//cout<<"---------------"<<endl;
	}
	return 0;
}
