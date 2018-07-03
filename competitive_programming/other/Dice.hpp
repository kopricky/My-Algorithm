#include "../header.hpp"

//上:0,下:1,右:2,左:3,手前:4,奥:5
struct Dice{
    vector<int> f={1,6,5,2,3,4};
    //回転後に上になる面を返す
    int Rotate(int dir,bool plus){
        //(+X,+Y,Z(上から見て左回り) 方向へrotate, plus=falseなら-方向)
        if(plus){
            if(dir == 0){
                swap(f[0],f[2]),swap(f[0],f[1]),swap(f[0],f[3]);
            }else if(dir == 1){
                swap(f[0],f[5]),swap(f[0],f[1]),swap(f[0],f[4]);
            }else{
                swap(f[2],f[5]),swap(f[2],f[3]),swap(f[2],f[4]);
            }
        }else{
            if(dir == 0){
                swap(f[0],f[3]),swap(f[0],f[1]),swap(f[0],f[2]);
            }else if(dir == 1){
                swap(f[0],f[4]),swap(f[0],f[1]),swap(f[0],f[5]);
            }else{
                swap(f[2],f[4]),swap(f[2],f[3]),swap(f[2],f[5]);
            }
        }
        return f[0];
    }
    //face[id1]=num1,face[id2]=num2になるむきにサイコロを回転させる
    //2面でサイコロの向きは決定するので
    void Arrange(int id1, int num1, int id2, int num2){
        while(1){
            if(f[id1] == num1 && f[id2] == num2){
                return;
            }
            Rotate(rand()%3,true);
        }
    }
};
