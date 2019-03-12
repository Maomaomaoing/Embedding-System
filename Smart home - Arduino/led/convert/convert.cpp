int convert::charToInt(char a, char b){
    
    int c,d;
    
    c=a;
    d=b;
    
    if(c>64) c-=55;
    else c-=48;
    if(d>64) -=55;
    else d-=48;
    
    return c*16+d;
    }
