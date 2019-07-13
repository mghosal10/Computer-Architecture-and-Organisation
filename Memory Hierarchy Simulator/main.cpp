//
//  main.cpp
//  CacheSimulator
//
//  Created by Madhumita Ghosal on 4/18/19.
//  Copyright © 2019 Madhumita Ghosal. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <stdexcept>


using namespace std;

int binary_to_decimal(string binary);
string convert_dec2hex(unsigned int address);

// hexadecimal to binary code referred from geeksforgeeks
string convert_hex2bin(string hexadecimal)
{
    int i = 0;
    string bin_num, ith_hex_val;
    while(hexadecimal[i])
    {
        switch(hexadecimal[i])
        {
            case '0':
                ith_hex_val = "0000";
                break;
                
            case '1':
                ith_hex_val = "0001";
                break;
                
            case '2':
                ith_hex_val = "0010";
                break;
                
            case '3':
                ith_hex_val = "0011";
                break;
                
            case '4':
                ith_hex_val = "0100";
                break;
                
            case '5':
                ith_hex_val = "0101";
                break;
                
            case '6':
                ith_hex_val = "0110";
                break;
                
            case '7':
                ith_hex_val = "0111";
                break;
                
            case '8':
                ith_hex_val = "1000";
                break;
                
            case '9':
                ith_hex_val = "1001";
                // cout << "1001";
                break;
                
            case 'A':
                ith_hex_val = "1010";
                break;
                
            case 'B':
                ith_hex_val = "1011";
                break;
                
            case 'C':
                ith_hex_val = "1100";
                break;
                
            case 'D':
                ith_hex_val = "1101";
                break;
                
            case 'E':
                ith_hex_val = "1110";
                break;
                
            case 'F':
                ith_hex_val = "1111";
                break;
                
            case 'a':
                ith_hex_val = "1010";
                break;
                
            case 'b':
                ith_hex_val = "1011";
                // cout << "1011";
                break;
                
            case 'c':
                ith_hex_val = "1100";
                break;
                
            case 'd':
                ith_hex_val = "1101";
                break;
                
            case 'e':
                ith_hex_val = "1110";
                break;
                
            case 'f':
                ith_hex_val = "1111";
                break;
                
            default:
                cout << "\nInvalid hexadecimal number" << hexadecimal[i];
        }
        bin_num = bin_num+ith_hex_val;
        i++;
    }
    // cout << "The hex2bin num is " << bin_num <<"\n";
    return bin_num;
}

// decimal to hexadecimal code referred from geeksforgeeks
string convert_dec2hex(unsigned int address)
{
    char arr[100];
    char hex_arr[100] = {0};
    string hexadecimal;
    int i=0;
    int a=0;
    while(address>0)
    {
        int r = address%16;
        if(r < 10)
        {
            arr[i] = r+48;
            i++;
        }
        else
        {
            arr[i] = r+55;
            i++;
        }
        address = address/16;
    }
    for(int j=i-1; j>=0; j--)
    {
        hex_arr[a] = arr[j];
        a++;
        // cout << arr[j];
    }
    
    hexadecimal = string(hex_arr);
    // cout << "\n" << hexadecimal << "\n";
    return hexadecimal;
}

int binary_to_decimal(string binary) {
    int decimal = 0;
    int power = 0;
    for(int i = binary.length()-1;i>=0;i--){
        if(binary[i] == '1') {
            decimal += pow(2,power);
        }
        power++;
    }
    return decimal;
}



int main(int argc, const char * argv[])
{
    
    FILE *fp;
    char readwrite;
    unsigned int address;
    string hexadecimal_num, bin_num;
    
    int blocksize = atoi(argv[1]);
    int l1_size = atoi(argv[2]);
    int l1_assoc = atoi(argv[3]);
    int l2_size = atoi(argv[4]);
    int l2_assoc = atoi(argv[5]);
    string replacement_policy = argv[6];
    string inclusion_property = argv[7];
    string filename = argv[8];
    
    int l1_readmiss = 0, l2_read = 0, l1_write_misses = 0, l2_read_misses = 0, l2_write = 0, l2_write_misses = 0, l1_read = 0, l1_write = 0;
    float l1_missrate = 0; int L1_write_bcks=0; int L2_write_bcks=0;
    
    int policy_counter = 1;
    int policy_counter_l2 = 1;
    
    if (blocksize == 0) {
        blocksize = 1;
    }
    
    int l1_sets = l1_size/(l1_assoc*blocksize);
    string L1[l1_sets][l1_assoc];
    //  unsigned int lineNumber[l1_sets][l1_assoc];
    int dirtyBitL1[l1_sets][l1_assoc];
    int policyL1[l1_sets][l1_assoc];
    
    int l2_sets = 0;
    
    if(l2_assoc != 0 ) {
        l2_sets = l2_size/(l2_assoc*blocksize);
    }
    
    
    
    string L2[l2_sets][l2_assoc];
    int dirtyBitL2[l2_sets][l2_assoc];
    int policyL2[l2_sets][l2_assoc];
    
    // initialize L1
    for (int i = 0; i<l1_sets; i++) {
        for (int j = 0; j<l1_assoc; j++){
            L1[i][j] = "";        // initial value to L1
            dirtyBitL1[i][j] = 0; // 0 the corresoponding l1 is not filled, 1 data is ready, 2 data updated
            policyL1[i][j] = 0;
            // lineNumber[i][j] = 0;
        }
    }
    
    // initialize L2
    for (int i = 0; i<l2_sets; i++) {
        for (int j = 0; j<l2_assoc; j++){
            L2[i][j] = "";        // initial value to L2
            dirtyBitL2[i][j] = 0; // 0 the corresoponding l1 is not filled, 1 data is ready, 2 data updated
            policyL2[i][j] = 0;
        }
    }
    
    cout << "\n===== Simulator configuration =====";
    cout << "\nBLOCKSIZE: " << blocksize;
    cout << "\nL1_SIZE: " << l1_size;
    cout << "\nL1_ASSOC: " << l1_assoc;
    cout << "\nL2_SIZE: " << l2_size ;
    cout << "\nL2_ASSOC: " << l2_assoc;
    if(replacement_policy.compare("0") == 0)
    {
        cout << "\nREPLACEMENT POLICY: LRU";
    }
    if(inclusion_property.compare("0") == 0)
    {
        cout << "\nINCLUSION PROPERTY: non-inclusive";
    }
    cout << "\ntrace_file: " << filename;
    
    fp = fopen("/Users/madhumitaghosal/Documents/Code/CacheSimulator/CacheSimulator/gcc_trace.txt", "r");
    
    if(fp != NULL)
    {
        while(fscanf(fp, "%c %x\n", &readwrite, &address) != EOF)
        {
            hexadecimal_num = convert_dec2hex(address);
            bin_num = convert_hex2bin(hexadecimal_num);
            
            int offset = log2(blocksize);
            // cout << "the offset is " << offset <<"\n";
            
            int bin_len = bin_num.length();
            int removed_len = bin_len-offset;
            string new_bin_str = bin_num.substr(0, removed_len);
            //  cout << "\n str : " << new_bin_str;
            
            int l1_index_bits = log2(l1_sets);
            int new_bin_str_len = new_bin_str.length();
            
            int bin_index_bit = new_bin_str_len-l1_index_bits;
            string index = new_bin_str.substr(bin_index_bit, new_bin_str_len);
            int l1_new_index = binary_to_decimal(index);
            
            int l2_index_bits,l2_bin_str_len, l2bin_index_bit, l2_new_index;
            string l2index;
            
            // L2 cache
            if(l2_sets != 0)
            {
                l2_index_bits = log2(l2_sets);
                l2_bin_str_len = new_bin_str.length();
                l2bin_index_bit = l2_bin_str_len-l2_index_bits;
                l2index = new_bin_str.substr(l2bin_index_bit, l2_bin_str_len);
                l2_new_index = binary_to_decimal(l2index);
            }
            
            if(readwrite == 'r')
            {
                l1_read++;
            }
            else if (readwrite == 'w')
            {
                l1_write++;
            }
            
            int updated = 0, updated_l2;
            string value = convert_dec2hex(binary_to_decimal(new_bin_str.substr(0,bin_index_bit)));
            string l2_value;
            if(l2_sets != 0)
                l2_value = convert_dec2hex(binary_to_decimal(new_bin_str.substr(0,l2bin_index_bit)));
            
            int myIndex;
            int changed = 0, changed_l2;
            for(int k = 0; k<l1_assoc; k++)
            {
                if(L1[l1_new_index][k].compare(value) == 0)
                {
                    myIndex = k;
                    updated = 1;
                    break;
                }
                
            }
            bool is_L2_read = false;
            if(updated==0){
                if(readwrite == 'r')
                    l1_readmiss++;
                else
                    l1_write_misses++;
                is_L2_read = true;
            }
            if (updated==0)
            {
                for(int k = 0; k<l1_assoc; k++)
                {
                    if(dirtyBitL1[l1_new_index][k]==0)
                    {
                        myIndex = k;
                        updated = 1;
                        changed = 1;
                    }
                }
            }
            
            bool is_l2_write = false;
            string L2_write_val;
            if (updated == 0)
            {
                // update policy
                
                //LRU
                if(replacement_policy.compare("0") == 0){
                    int min = 0;
                    for (int i =1; i<l1_assoc; i++) {
                        if(policyL1[l1_new_index][i] < policyL1[l1_new_index][min]) {
                            min = i;
                        }
                    }
                    myIndex = min;
                    updated = 1;
                    changed = 1;
                    if(dirtyBitL1[l1_new_index][myIndex]==2){
                        L1_write_bcks++;
                        is_l2_write = true;
                        L2_write_val = L1[l1_new_index][myIndex];
                    }
                }
            }
            
            if(is_l2_write && l2_sets != 0)
            {
                // write in L2
                l2_write++;
                int diff = l2_index_bits - l1_index_bits;
                string l1_tag = convert_hex2bin(L2_write_val);
                string l2_tag = l1_tag.substr(0, l1_tag.length()-diff);
                string new_l2_tag = convert_dec2hex(binary_to_decimal(l2_tag));
                
                string last_bit = l1_tag.substr(l1_tag.length()-diff, l1_tag.length());
                //string bin_new_l2index = l2index.append(last_bit);
                string bin_new_l2index = last_bit+index;
                int new_l2index = binary_to_decimal(bin_new_l2index);
                if(l2_sets != 0)
                {
                    updated_l2 = 0;
                    int myIndex_l2;
                    changed_l2 = 0;
                    for(int k = 0; k<l2_assoc; k++)
                    {
                        if(L2[new_l2index][k].compare(new_l2_tag) == 0)
                        {
                            myIndex_l2 = k;
                            updated_l2 = 1;
                        }
                    }
                    
                    if (updated_l2 == 0)
                    {
                        l2_write_misses++;
                        for(int k = 0; k<l2_assoc; k++)
                        {
                            if(dirtyBitL2[new_l2index][k]==0)
                            {
                                myIndex_l2 = k;
                                updated_l2 = 1;
                                changed_l2 = 1;
                            }
                        }
                    }
                    
                    if (updated_l2 == 0)
                    {
                        //LRU for L2
                        if(replacement_policy.compare("0") == 0)
                        {
                            int min = 0;
                            for (int i = 1; i<l2_assoc; i++)
                            {
                                if(policyL2[new_l2index][i] < policyL2[new_l2index][min])
                                {
                                    min = i;
                                }
                            }
                            myIndex_l2 = min;
                            updated_l2 = 1;
                            changed_l2 = 1;
                            
                            if(dirtyBitL2[new_l2index][myIndex_l2]==2)
                                L2_write_bcks++;
                        }
                    }
                    L2[new_l2index][myIndex_l2] = new_l2_tag;
                    dirtyBitL2[new_l2index][myIndex_l2] = 2;
                    policyL2[new_l2index][myIndex_l2] = policy_counter_l2++;
                }
                
            }
            
            //L2
            if(is_L2_read && l2_sets != 0)
            {
                //l1_readmiss++;
                l2_read++;
                
                if(l2_sets != 0)
                {
                    updated_l2 = 0;
                    int myIndex_l2;
                    changed_l2 = 0;
                    for(int k = 0; k<l2_assoc; k++)
                    {
                        if(L2[l2_new_index][k].compare(l2_value) == 0)
                        {
                            myIndex_l2 = k;
                            updated_l2 = 1;
                            break;
                        }
                    }
                    
                    if (updated_l2 == 0)
                    {
                        l2_read_misses++;
                        for(int k = 0; k<l2_assoc; k++)
                        {
                            if(dirtyBitL2[l2_new_index][k]==0)
                            {
                                myIndex_l2 = k;
                                updated_l2 = 1;
                                changed_l2 = 1;
                            }
                        }
                    }
                    bool is_evicted_w=false;
                    if (updated_l2 == 0)
                    {
                        //LRU for L2
                        if(replacement_policy.compare("0") == 0)
                        {
                            int min = 0;
                            for (int i = 1; i<l2_assoc; i++)
                            {
                                if(policyL2[l2_new_index][i] < policyL2[l2_new_index][min])
                                {
                                    min = i;
                                }
                            }
                            myIndex_l2 = min;
                            updated_l2 = 1;
                            changed_l2 = 1;
                            if(dirtyBitL2[l2_new_index][myIndex_l2]==2){
                                is_evicted_w=true;
                                L2_write_bcks++;
                            }
                            
                        }
                    }
                    L2[l2_new_index][myIndex_l2] = l2_value;
                    if(dirtyBitL2[l2_new_index][myIndex_l2]!=2 || is_evicted_w)
                        dirtyBitL2[l2_new_index][myIndex_l2] = 1;
                    policyL2[l2_new_index][myIndex_l2] = policy_counter_l2++;
                }
                
            }
            L1[l1_new_index][myIndex] = value;
            if (readwrite == 'w') // if dirty bit is 2 write to l2
            {
                dirtyBitL1[l1_new_index][myIndex] = 2;
            }
            else if (readwrite == 'r' && changed == 1)
            {
                dirtyBitL1[l1_new_index][myIndex] = 1;
            }
            policyL1[l1_new_index][myIndex] = policy_counter++;
            
        }
        
        // print L1 cache
        cout << "\n\n===== L1 contents ===== ";
        for(int i = 0; i<l1_sets; i++) {
            cout <<"\nSet "<< i << " : ";
            for(int j = l1_assoc-1; j>=0; j--)
            {
                cout<< L1[i][j] << " ";
                if(dirtyBitL1[i][j]==2)
                {
                    cout << "D     ";
                }
                else {
                    cout << "      ";
                }
            }
        }
        
        // print L2 cache
        if(l2_sets != 0)
        {
            cout << "\n\n===== L2 contents ===== ";
            for(int i = 0; i<l2_sets; i++) {
                cout <<"\nSet "<< i << " : ";
                // for(int j = 0; j<l2_assoc; j++)
                for(int j = l2_assoc-1; j>=0; j--)
                {
                    cout<< L2[i][j] << " ";
                    if(dirtyBitL2[i][j] == 2)
                    {
                        cout << "D     ";
                    }
                    else
                    {
                        cout << "      ";
                    }
                }
            }
        }
    }
    else
    {
        cout << "\nCannot open file";
        exit(0);
    }
    
    if(l1_readmiss != 0 || l1_write_misses != 0)
    {
        l1_missrate = (float)(l1_readmiss+l1_write_misses)/(l1_read+l1_write);
    }
    float l2_missrate =0;
    if(l2_sets != 0)
        l2_missrate = (float)(l2_read_misses)/(l2_read);
    cout << "\n\n===== Simulation results (raw) =====";
    cout << "\na. number of L1 reads:  " << l1_read;
    cout << "\nb. number of L1 read misses: " << l1_readmiss;
    cout << "\nc. number of L1 writes:  " << l1_write;
    cout << "\nd. number of L1 write misses: " << l1_write_misses;
    cout << "\ne. L1 miss rate: " << l1_missrate;
    cout << "\nf. number of L1 writebacks: "<<L1_write_bcks;
    cout << "\ng. number of L2 reads: " << l2_read;
    cout << "\nh. number of L2 read misses: " << l2_read_misses;
    cout << "\ni. number of L2 writes: " << l2_write;
    cout << "\nj. number of L2 write misses: " << l2_write_misses;
    cout << "\nk. L2 miss rate: "<<l2_missrate;
    cout << "\nl. number of L2 writebacks: "<<L2_write_bcks;
    int mem_traffic = 0;
    if(l2_sets != 0)
        mem_traffic = l2_read_misses + l2_write_misses + L2_write_bcks;
    else
        mem_traffic = l1_readmiss + l1_write_misses + L1_write_bcks;
    cout << "\nm. total memory traffic: "<<mem_traffic<<endl;
    
    
}
