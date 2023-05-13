int getAddr(uint32_t x, uint32_t y){
   return (LEDsH * ((LEDsW-1)-x)) + (((x&1)==0)?((LEDsH-1)-y):(y));
  //return (y*LEDsW) + ( ((y&1)==0) ? x : ((LEDsW-1)-x) );
}

byte all[] = {
//[space]
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,

//!
0002, //1,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,
0002, //1,0,
0000, //0,0,

//"
0012, //0,1,0,1,0,
0012, //0,1,0,1,0,
0024, //1,0,1,0,0,
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,

//#
0024, //0,1,0,1,0,0,
0024, //0,1,0,1,0,0,
0076, //1,1,1,1,1,0,
0024, //0,1,0,1,0,0,
0076, //1,1,1,1,1,0,
0024, //0,1,0,1,0,0,
0024, //0,1,0,1,0,0,
0000, //0,0,0,0,0,0,

//$
0010, //0,0,1,0,0,0,
0036, //0,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0002, //0,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//%
0042, //1,0,0,0,1,0,
0044, //1,0,0,1,0,0,
0004, //0,0,0,1,0,0,
0010, //0,0,1,0,0,0,
0020, //0,1,0,0,0,0,
0022, //0,1,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//&
0010, //0,0,1,0,0,0,
0024, //0,1,0,1,0,0,
0010, //0,0,1,0,0,0,
0032, //0,1,1,0,1,0,
0054, //1,0,1,1,0,0,
0044, //1,0,0,1,0,0,
0032, //0,1,1,0,1,0,
0000, //0,0,0,0,0,0,

//'
0002, //0,1,0,
0002, //0,1,0,
0004, //1,0,0,
0000, //0,0,0,
0000, //0,0,0,
0000, //0,0,0,
0000, //0,0,0,
0000, //0,0,0,

//(
0006, //0,0,1,1,0,
0010, //0,1,0,0,0,
0020, //1,0,0,0,0,
0020, //1,0,0,0,0,
0020, //1,0,0,0,0,
0010, //0,1,0,0,0,
0006, //0,0,1,1,0,
0000, //0,0,0,0,0,

//)
0030, //1,1,0,0,0,
0004, //0,0,1,0,0,
0002, //0,0,0,1,0,
0002, //0,0,0,1,0,
0002, //0,0,0,1,0,
0004, //0,0,1,0,0,
0030, //1,1,0,0,0,
0000, //0,0,0,0,0,

//*
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,
0022, //1,0,0,1,0,
0014, //0,1,1,0,0,
0022, //1,0,0,1,0,
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,
0000, //0,0,0,0,0,

//+
0000, //0,0,0,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0076, //1,1,1,1,1,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,

//-
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//.
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0000, //0,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,

///
0002, //0,0,0,0,1,0,
0004, //0,0,0,1,0,0,
0004, //0,0,0,1,0,0,
0010, //0,0,1,0,0,0,
0020, //0,1,0,0,0,0,
0020, //0,1,0,0,0,0,
0040, //1,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//0
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0046, //1,0,0,1,1,0,
0052, //1,0,1,0,1,0,
0062, //1,1,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//1
0010, //0,0,1,0,0,0,
0030, //0,1,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//2
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0014, //0,0,1,1,0,0,
0020, //0,1,0,0,0,0,
0042, //1,0,0,0,1,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//3
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0014, //0,0,1,1,0,0,
0002, //0,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//4
0006, //0,0,0,1,1,0,
0012, //0,0,1,0,1,0,
0022, //0,1,0,0,1,0,
0042, //1,0,0,0,1,0,
0076, //1,1,1,1,1,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//5
0076, //1,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0074, //1,1,1,1,0,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//6
0014, //0,0,1,1,0,0,
0020, //0,1,0,0,0,0,
0040, //1,0,0,0,0,0,
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//7
0076, //1,1,1,1,1,0,
0042, //1,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0004, //0,0,0,1,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//8
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//9
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0002, //0,0,0,0,1,0,
0004, //0,0,0,1,0,0,
0030, //0,1,1,0,0,0,
0000, //0,0,0,0,0,0,

//:
0000, //0,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,
0000, //0,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,

//;
0000, //0,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,
0000, //0,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,

//<
0002, //0,0,0,1,0,
0004, //0,0,1,0,0,
0010, //0,1,0,0,0,
0020, //1,0,0,0,0,
0010, //0,1,0,0,0,
0004, //0,0,1,0,0,
0002, //0,0,0,1,0,
0000, //0,0,0,0,0,

//=
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//>
0020, //1,0,0,0,0,
0010, //0,1,0,0,0,
0004, //0,0,1,0,0,
0002, //0,0,0,1,0,
0004, //0,0,1,0,0,
0010, //0,1,0,0,0,
0020, //1,0,0,0,0,
0000, //0,0,0,0,0,

//?
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0004, //0,0,0,1,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//@
0074, //0,1,1,1,1,0,0,
0102, //1,0,0,0,0,1,0,
0132, //1,0,1,1,0,1,0,
0132, //1,0,1,1,0,1,0,
0136, //1,0,1,1,1,1,0,
0100, //1,0,0,0,0,0,0,
0074, //0,1,1,1,1,0,0,
0000, //0,0,0,0,0,0,0,

//A
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0076, //1,1,1,1,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//B
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//C
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//D
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//E
0076, //1,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0070, //1,1,1,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//F
0076, //1,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0070, //1,1,1,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//G
0036, //0,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0056, //1,0,1,1,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//H
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0076, //1,1,1,1,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//I
0016, //1,1,1,0,
0004, //0,1,0,0,
0004, //0,1,0,0,
0004, //0,1,0,0,
0004, //0,1,0,0,
0004, //0,1,0,0,
0016, //1,1,1,0,
0000, //0,0,0,0,

//J
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//K
0042, //1,0,0,0,1,0,
0044, //1,0,0,1,0,0,
0070, //1,1,1,0,0,0,
0044, //1,0,0,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//L
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//M
0042, //1,0,0,0,1,0,
0066, //1,1,0,1,1,0,
0052, //1,0,1,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//N
0042, //1,0,0,0,1,0,
0062, //1,1,0,0,1,0,
0052, //1,0,1,0,1,0,
0046, //1,0,0,1,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//O
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//P
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//Q
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0044, //1,0,0,1,0,0,
0032, //0,1,1,0,1,0,
0000, //0,0,0,0,0,0,

//R
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//S
0036, //0,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//T
0076, //1,1,1,1,1,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//U
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//V
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0024, //0,1,0,1,0,0,
0024, //0,1,0,1,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//W
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0052, //1,0,1,0,1,0,
0066, //1,1,0,1,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//X
0042, //1,0,0,0,1,0,
0024, //0,1,0,1,0,0,
0010, //0,0,1,0,0,0,
0024, //0,1,0,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//Y
0042, //1,0,0,0,1,0,
0024, //0,1,0,1,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//Z
0076, //1,1,1,1,1,0,
0002, //0,0,0,0,1,0,
0004, //0,0,0,1,0,0,
0010, //0,0,1,0,0,0,
0020, //0,1,0,0,0,0,
0040, //1,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//[
0016, //1,1,1,0,
0010, //1,0,0,0,
0010, //1,0,0,0,
0010, //1,0,0,0,
0010, //1,0,0,0,
0010, //1,0,0,0,
0016, //1,1,1,0,
0000, //0,0,0,0,

/* \ special comment for a special character */
0040, //1,0,0,0,0,0,
0020, //0,1,0,0,0,0,
0020, //0,1,0,0,0,0,
0010, //0,0,1,0,0,0,
0004, //0,0,0,1,0,0,
0004, //0,0,0,1,0,0,
0002, //0,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//]
0016, //1,1,1,0,
0002, //0,0,1,0,
0002, //0,0,1,0,
0002, //0,0,1,0,
0002, //0,0,1,0,
0002, //0,0,1,0,
0016, //1,1,1,0,
0000, //0,0,0,0,

//^
0010, //0,0,1,0,0,0,
0024, //0,1,0,1,0,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//_
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//`
0004, //1,0,0,
0004, //1,0,0,
0002, //0,1,0,
0000, //0,0,0,
0000, //0,0,0,
0000, //0,0,0,
0000, //0,0,0,
0000, //0,0,0,

//a
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0002, //0,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//b
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0054, //1,0,1,1,0,0,
0062, //1,1,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//c
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0040, //1,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//d
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0032, //0,1,1,0,1,0,
0046, //1,0,0,1,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//e
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0076, //1,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0036, //0,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//f
0006, //0,0,1,1,0,
0010, //0,1,0,0,0,
0036, //1,1,1,1,0,
0010, //0,1,0,0,0,
0010, //0,1,0,0,0,
0010, //0,1,0,0,0,
0010, //0,1,0,0,0,
0000, //0,0,0,0,0,

//g
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0036, //0,1,1,1,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0002, //0,0,0,0,1,0,
0074, //1,1,1,1,0,0,

//h
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0054, //1,0,1,1,0,0,
0062, //1,1,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//i
0002, //1,0,
0000, //0,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,

//j
0002, //0,0,0,0,1,0,
0000, //0,0,0,0,0,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,

//k
0020, //1,0,0,0,0,
0020, //1,0,0,0,0,
0022, //1,0,0,1,0,
0024, //1,0,1,0,0,
0030, //1,1,0,0,0,
0024, //1,0,1,0,0,
0022, //1,0,0,1,0,
0000, //0,0,0,0,0,

//l
0004, //1,0,0,
0004, //1,0,0,
0004, //1,0,0,
0004, //1,0,0,
0004, //1,0,0,
0004, //1,0,0,
0002, //0,1,0,
0000, //0,0,0,

//m
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0064, //1,1,0,1,0,0,
0052, //1,0,1,0,1,0,
0052, //1,0,1,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//n
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0074, //1,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//o
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0034, //0,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//p
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0054, //1,0,1,1,0,0,
0062, //1,1,0,0,1,0,
0042, //1,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,

//q
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0032, //0,1,1,0,1,0,
0046, //1,0,0,1,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0002, //0,0,0,0,1,0,
0002, //0,0,0,0,1,0,

//r
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0054, //1,0,1,1,0,0,
0062, //1,1,0,0,1,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0040, //1,0,0,0,0,0,
0000, //0,0,0,0,0,0,

//s
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0036, //0,1,1,1,1,0,
0040, //1,0,0,0,0,0,
0034, //0,1,1,1,0,0,
0002, //0,0,0,0,1,0,
0074, //1,1,1,1,0,0,
0000, //0,0,0,0,0,0,

//t
0004, //0,1,0,0,
0004, //0,1,0,0,
0016, //1,1,1,0,
0004, //0,1,0,0,
0004, //0,1,0,0,
0004, //0,1,0,0,
0002, //0,0,1,0,
0000, //0,0,0,0,

//u
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//v
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0024, //0,1,0,1,0,0,
0010, //0,0,1,0,0,0,
0000, //0,0,0,0,0,0,

//w
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0052, //1,0,1,0,1,0,
0052, //1,0,1,0,1,0,
0036, //0,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//x
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0024, //0,1,0,1,0,0,
0010, //0,0,1,0,0,0,
0024, //0,1,0,1,0,0,
0042, //1,0,0,0,1,0,
0000, //0,0,0,0,0,0,

//y
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0042, //1,0,0,0,1,0,
0036, //0,1,1,1,1,0,
0002, //0,0,0,0,1,0,
0074, //1,1,1,1,0,0,

//z
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0076, //1,1,1,1,1,0,
0004, //0,0,0,1,0,0,
0010, //0,0,1,0,0,0,
0020, //0,1,0,0,0,0,
0076, //1,1,1,1,1,0,
0000, //0,0,0,0,0,0,

//{
0006, //0,0,1,1,0,
0010, //0,1,0,0,0,
0010, //0,1,0,0,0,
0020, //1,0,0,0,0,
0010, //0,1,0,0,0,
0010, //0,1,0,0,0,
0006, //0,0,1,1,0,
0000, //0,0,0,0,0,

//|
0002, //1,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,
0002, //1,0,
0002, //1,0,
0002, //1,0,
0000, //0,0,

//}
0030, //1,1,0,0,0,
0004, //0,0,1,0,0,
0004, //0,0,1,0,0,
0002, //0,0,0,1,0,
0004, //0,0,1,0,0,
0004, //0,0,1,0,0,
0030, //1,1,0,0,0,
0000, //0,0,0,0,0,

//~
0031, //0,1,1,0,0,1,
0046, //1,0,0,1,1,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
0000, //0,0,0,0,0,0,
};

int lengths[] = {2,2,5,6,6,6,6,3,5,5,5,6,2,6,2,6,6,6,6,6,6,6,6,6,6,6,2,2,5,6,5,6,7,6,6,6,6,6,6,6,6,4,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,4,6,4,6,6,3,6,6,6,6,6,5,6,6,2,6,5,3,6,6,6,6,6,6,6,4,6,6,6,6,6,6,5,2,5,6};

int getCharPixel(char thisChar, int down, int left){
    //If they somehow managed to get a character before the ascii space, make it an ascii space
    if (thisChar < 32){
        thisChar = ' ';
    }
    
    String row = String(all[((thisChar-32)*8)+down], BIN);
    
    int rowLength = row.length();
    int length = lengths[thisChar - 32];
    
    if(rowLength < length && (left < (length - rowLength))){
        return 0;
    }
    else {
        int indexOfPixel = left - (length - rowLength);
        return row.substring(indexOfPixel, indexOfPixel+1).toInt();
    }
}

void displayTextOnPanel(const char* strIn, size_t strLen, uint32_t color, Adafruit_NeoPixel& strip){
    int maxWidth = 0;
    int offset = 0;
    for (int i = 0; i < strLen; i++){
        char thisChar = strIn[i];
        maxWidth += lengths[thisChar-32];
    }

    if (maxWidth > LEDsW) {
      maxWidth = LEDsW;
    } else {
      offset = floor((LEDsW - maxWidth) / 2);
    }
    
    //start at the 0th character, on the 0th row
    int charOffset = 0;
    int charColOffset = 0;
    
    for (int column = 0; column < maxWidth; column++){
        char thisChar = strIn[charOffset];
        int innerCharOffset = charOffset;
        int innerCharColOffset = charColOffset;
        
        for(int x=(maxWidth-1)+offset; x>=offset; x--){
          char innerThisChar = strIn[innerCharOffset];
          for(int y=0; y<LEDsH; y++){
            int pixel = getCharPixel(innerThisChar, y, innerCharColOffset);
            int addr = getAddr(x, y);
            uint32_t showColor = pixel == 1 ? color : strip.Color(0,0,0);
            strip.setPixelColor(addr, showColor);
          }
          
          innerCharColOffset++;
          if (innerCharColOffset > (lengths[innerThisChar - 32] -1)){
            innerCharOffset++;
                
            if (innerCharOffset > (strLen-1)){
              innerCharOffset = 0;
            }
                
            innerCharColOffset = 0;
          }
        }
    }
    strip.show();
}
