//
//  main.cpp
//  branch_predictor
//
//  Created by Madhumita Ghosal on 2/28/19.
//  Copyright © 2019 Madhumita Ghosal. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <string>

using namespace std;

int hybrid_index(int dec, int m1, int m2, int n, int k, unsigned int address)
{
   /*int result = 0;
    char bin[256] ;
    strcpy(bin,"");
    while(dec > 0)
    {
        char re[1];
        sprintf(re, "%d", dec%2);
        strcat(bin,re);
        dec = dec/2;
    }

    for(int i = k+2;i>2;i--)
    {
        int t =  bin[i] - '0';
        result = result*2 + t;
    }
  
    //cout << "......." << result;
    return result;*/
    
    return (address & (int)(pow(2,k)-1));
}

string convert_to_bin(int dec, int m, int n)
{
    char bin[256] ;
    string n_bit;
    strcpy(bin,"");
    while(dec > 0)
    {
        char re[1];
        sprintf(re, "%d", dec%2);
        strcat(bin,re);
        dec = dec/2;
    }
    
    for(int i=2; i<m+2; i++)
    {
        n_bit = n_bit+bin[i];
    }
 
    return n_bit;
}

int convert(int dec, int m)
{
    char bin[256] ;
    strcpy(bin,".");
    int result = 0;
    while(dec > 0)
    {
        char re[1];
        sprintf(re, "%d", dec%2);
        strcat(bin,re);
        dec = dec/2;
    }
    
    for(int i = m+2;i>2;i--)
    {
        int t =  bin[i] - '0';
        result = result*2 + t;
    }
    return result;
}

int call_bimodal(unsigned int address, char  decision, int m)
{
    vector <int> arr;
    int table_size = pow(2, m);
    int prediction=0, mispredictions=0;
    unsigned int addr;
    int  decisn;
    
    for(int i = 0; i<table_size ; i++)
    {
        arr.push_back(4);
    }
    
    int res = convert(address,m);
    
    if(decision == 't')
    {
        decisn = 1;
        if(arr[res] >= 4 && arr[res] <= 7)
        {
            prediction = 1;
            arr[res] = arr[res]+1;
            
        }
    }
    else if (decision == 'n')
    {
        decisn = 0;
        if(arr[res] >= 0 && arr[res] <= 3)
        {
            prediction = 0;
            arr[res] = arr[res]-1;
            
        }
    }
    
    if(decision != prediction)
    {
        mispredictions++;
    }
    
    return prediction;
}

int call_gshare(unsigned int address, char  decision, int m, int n, char *global_history_reg)
{
    vector <int> arr;
    int table_size = pow(2, m);
    int prediction;
    unsigned int addr;
    char  decisn;
    string res;
    char xorResult[n];
    
    for(int i = 0; i<table_size ; i++)
    {
        arr.push_back(4);
    }
    
    res = convert_to_bin(address, m, n);
    
    for(int i=0; i<n; i++)
    {
        if(res[i] == '0' && global_history_reg[i] == '0') {
            xorResult[i] = '0';
        }
        if(res[i] == '1' && global_history_reg[i] == '0') {
            xorResult[i] = '1';
        }
        if(res[i] == '1' && global_history_reg[i] == '1') {
            xorResult[i] = '0';
        }
        if(res[i] == '0' && global_history_reg[i] == '1') {
            xorResult[i] = '1';
        }
    }
    
    int temp = 0;
    for(int i = 0;i<n;i++) {
        
        temp = temp+pow(2,i)*(xorResult[i]-'0');
    }
    for(int i = n;i<m;i++) {
        temp = temp+pow(2,i)*(res[i]-'0');
    }
    
    if(decision == 't')
    {
        if(arr[temp] >=4 && arr[temp] <= 7)
        {
            prediction = 1;
            arr[temp] = arr[temp]+1;
            
        }
    }
    else if (decision == 'n')
    {
        if(arr[temp] >=0 && arr[temp] <= 3)
        {
            prediction = 0;
            arr[temp] = arr[temp]-1;
            
        }
    }
    return prediction;
}

void bimodal_predictor(const char * t_file, int m)
{
    int total_predictions = 0, mispredictions = 0;
    double misprediction_rate = 0;
    unsigned int address;
    char  decision, output;
    vector <int> arr;
   
    FILE * fp;
    
    int table_size = pow(2, m);
    
    fp = fopen("/Users/madhumitaghosal/Documents/Code/branch_predictor/branch_predictor/gcc_trace.txt", "r");
    
    for(int i = 0; i<table_size ; i++)
    {
        arr.push_back(4);
    }

    if(fp != NULL)
    {
        while(fscanf(fp, "%x %c", &address, &decision) != EOF)
        {
            total_predictions++;
            int res = convert(address,m);
            
            if(decision == 't')
            {
                if(arr[res] <= 3){
                    mispredictions++;
                }
                if(arr[res] != 7)
                {
                    arr[res] = arr[res]+1;
                }
            }
            else if(decision == 'n')
            {
                if(arr[res] >= 4){
                    mispredictions++;
                }
                if(arr[res] != 0)
                {
                    arr[res] = arr[res]-1;
                }
            }
        }
    
        printf("FINAL  BIMODAL  CONTENTS \n");
        for(int i = 0; i<table_size; i++) {
            if(arr[i]>=4 && arr[i]<= 7)
            {
                output = 't';
                printf("%d  : %d : %c\n ",i,arr[i], output);
            }
            else if(arr[i]>=0 && arr[i] <= 3)
            {
                output = 'n';
                //printf("%d : %d : %c\n ",i,arr[i], output);
                printf("%d : %d\n ",i,arr[i]);
            }
        }
    }
    else
    {
        cout << "\n File cannot open!\n";
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    
    cout << "\nThe number of mispredictions are " << mispredictions;
    cout << "\nThe number of predictions are " << total_predictions;
    misprediction_rate = ((double)mispredictions/total_predictions)*100;
    cout << "\nThe misprediction rate is " << misprediction_rate;
}


void gshare_predictor(const char * t_file, int m, int n)
{
    vector <int> arr;
    char  decision, output;
    unsigned int address;
    char global_history_reg[n];
    int total_predictions = 0, mispredictions = 0;
    double misprediction_rate = 0;
    
    int table_size = pow(2, m);
    
    FILE * fp;
    fp = fopen("/Users/madhumitaghosal/Documents/Code/branch_predictor/branch_predictor/gcc_trace.txt", "r");
    
    for(int i = 0; i <table_size ; i++)
    {
        arr.push_back(4);
    }
    
    if(fp != NULL)
    {
        while(fscanf(fp, "%x %c", &address, &decision) != EOF)
        {
            string res;
            char xorResult[n];
            string nbit;
            
            total_predictions++;
            
            res = convert_to_bin(address, m, n);
            
            for(int i=0; i<n; i++)
            {
                if(res[i] == '0' && global_history_reg[i] == '0') {
                    xorResult[i] = '0';
                }
                if(res[i] == '1' && global_history_reg[i] == '0') {
                    xorResult[i] = '1';
                }
                if(res[i] == '1' && global_history_reg[i] == '1') {
                    xorResult[i] = '0';
                }
                if(res[i] == '0' && global_history_reg[i] == '1') {
                    xorResult[i] = '1';
                }
            }
            
            // cout<< xorResult << "    *******   " << global_history_reg<<"\n";
            int temp = 0;
            for(int i = 0;i<n;i++) {
                
                temp = temp+pow(2,i)*(xorResult[i]-'0');
            }
            for(int i = n;i<m;i++) {
                temp = temp+pow(2,i)*(res[i]-'0');
            }
            
            if(decision == 't')
            {
                if(arr[temp] <= 3){
                    mispredictions++;
                }
                if(arr[temp] != 7)
                {
                    arr[temp] = arr[temp]+1;
                }
            }
            else if(decision == 'n')
            {
                if(arr[temp] >= 4)
                {
                    mispredictions++;
                }
                if(arr[temp] != 0)
                {
                    arr[temp] = arr[temp]-1;
                }
            }
            
            for(int i = 0 ; i<n-1 ; i++)
            {
                global_history_reg[i] = global_history_reg[i+1];
            }
            global_history_reg[n-1] = '1';
            if(decision == 'n') {
                global_history_reg[n-1] = '0';
            }
        }
        
        printf("FINAL : Gshare : Decision %d\n",table_size);
        
        for(int i = 0; i<table_size; i++) {
            if(arr[i]>=4 && arr[i]<= 7)
            {
                output = 't';
                printf("%d %d\n ",i,arr[i]);
            }
            else if(arr[i]>=0 && arr[i] <= 3)
            {
                output = 'n';
                printf("%d %d\n ", i, arr[i]);
            }
        }
    }
    else
    {
        cout << "\n File cannot open!\n";
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    
    cout << "\nThe number of predictions are " << total_predictions;
    cout << "\nThe number of mispredictions are " << mispredictions;
    misprediction_rate = ((double)mispredictions/total_predictions)*100;
    cout << "\nThe misprediction rate is " << misprediction_rate;
}


void hybrid_predictor(const char *t_file, int k, int m1, int n, int m2)
{
    char  decision;
    int bimodal_predict=0, gshare_predict=0;
    unsigned int address;
    char global_history_reg[n];
    fill_n(global_history_reg, n, '0');
    int chooser_size = pow(2, k);
    int chooser_table[chooser_size];
    fill_n(chooser_table, chooser_size, 1);
    int decisn;
    int mispredictions, total_predictions, misprediction_rate ;
    
    FILE * fp = fopen("/Users/madhumitaghosal/Documents/Code/branch_predictor/branch_predictor/gcc_trace.txt", "r");
    
    if(fp != NULL)
    {
        while(fscanf(fp, "%x %c", &address, &decision) != EOF)
        {
            int index_bin = hybrid_index(address, m1, m2, n, k, address);
            
            total_predictions++;
            
            if(decision == 't')
            {
                decisn = 1;
            }
            else
            {
                decisn = 0;
            }
            
            if(chooser_table[index_bin] == 0 || chooser_table[index_bin] == 1)
            {
                bimodal_predict = call_bimodal(address, decision, m2);
                for(int i = 0 ; i<n-1 ; i++)
                {
                    global_history_reg[i] = global_history_reg[i+1];
                }
               // cout << "...." <<global_history_reg;
                global_history_reg[n-1] = '1';
                if(decision == 'n') {
                    global_history_reg[n-1] = '0';
                }
            }
            else if(chooser_table[index_bin] == 2 || chooser_table[index_bin] == 3)
            {
                gshare_predict = call_gshare(address, decision, m1, n, global_history_reg);
                for(int i = 0 ; i<n-1 ; i++)
                {
                    global_history_reg[i] = global_history_reg[i+1];
                }
                global_history_reg[n-1] = '1';
                if(decision == 'n') {
                    global_history_reg[n-1] = '0';
                }
            }
            
            
            if((bimodal_predict != decisn) && (gshare_predict == decisn))
            {
               if(chooser_table[index_bin] != 3)
               {
                   chooser_table[index_bin] = chooser_table[index_bin] + 1;
               }
            }
            else if((bimodal_predict == decisn) && (gshare_predict != decisn))
            {
                if(chooser_table[index_bin] != 0 )
                {
                    chooser_table[index_bin] = chooser_table[index_bin] - 1;
                }
            }
            
        }
        
        printf("FINAL : CHOOSER : Decision \n");
        for(int i = 0; i<chooser_size; i++) {
            printf("%d  %d \n" , i, chooser_table[i]);
        }
    }
    else
    {
        cout << "\n File cannot open!\n";
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    
    cout << "\nThe number of mispredictions are " << mispredictions;
    cout << "\nThe number of predictions are " << total_predictions;
    misprediction_rate = ((double)mispredictions/total_predictions)*100;
    cout << "\nThe misprediction rate is " << misprediction_rate;
}



int main(int argc, const char * argv[]) {
    
    int option;
    int m, m1, m2, n, k, table_size;
    const char * t_file;
    
    cout << "\n Enter : \n 1 - Bimodal Predictor \n 2 - Gshare Predictor \n 3 - Hybrid Predictor \n";
    cin >> option;
    
    switch(option)
    {
        case 1 :
            m = atoi(argv[3]);
            t_file = argv[4];
            bimodal_predictor(t_file,m);
            break;
            
        case 2 :
            m = atoi(argv[3]);
            n = atoi(argv[4]);
            table_size = pow(2, m);
            t_file = argv[5];
            gshare_predictor(t_file, m, n);
            break;
            
        case 3 :
            k = atoi(argv[3]);
            m1 = atoi(argv[4]);
            n = atoi(argv[5]);
            m2 = atoi(argv[6]);
            t_file = argv[7];
            hybrid_predictor(t_file, k, m1, n, m2);
            break;
            
        default:
            printf("\nOption not correct!");
            break;
    }
}

