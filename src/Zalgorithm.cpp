vector<int> Z_algorithm(string s){ // s の末尾に番兵を入れる！
    vector<int> z(s.size());
    z[0] = s.size();
    int i = 1, j = 0;
    while(i < int(s.size())){
        while(i + j < int(s.size()) && s[j] == s[i + j]) j++;
        z[i] = j;

        if(j == 0){
            i++;
            continue;
        }
        int k = 1;
        while(k < j && k + z[k] < j){
            z[i + k] = z[k];
            k++;
        }
        i += k;
        j -= k;
    }
    return z;
}