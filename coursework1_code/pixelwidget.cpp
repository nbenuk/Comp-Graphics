#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include <vector>

#include "pixelwidget.hpp"


void PixelWidget::DefinePixelValues(){ //add pixels here; write methods like this for the assignments
  SetPixel(10,30,RGBVal(255,0,0));
  //User input to set pixel here
  float h = 20, v = 20;
  unsigned int lit = 1;
  for (unsigned int i = 0; i < lit; i++){
      SetPixel(h,v,RGBVal(255,255,0));
  }
}

void PixelWidget::DrawLine(float ha, float va, float hb, float vb, const RGBVal& rgbA, const RGBVal& rgbB, int s){ //height a... height b
//    SetPixel(floor(ha),floor(va),rgbA);
//    SetPixel(floor(hb),floor(vb),rgbB);
   
    float Y,X;
    int pixels = 0;
    //x=a-1 + wt
    //ha = va * t
    //hb = ha + w
    float w;
    w= hb - ha;
    //x = ha + w*t
    float YW;
    YW = vb-va;
    //y = va + w*t
    float steps = 0;
    while (steps < 1) {
        Y = va + YW*steps;
        X = ha+ w*steps;
//            SetPixel(floor(X),floor(Y),RGBVal(100,100,100));
        steps = steps +0.001;//+ (s/1000);
        pixels ++;
    }

    float step;
    float scalar,other;
    step = 1.0/pixels;
    steps=0;
    scalar = 0;
    while (steps < 1) {
        int blue = rgbA._blue * scalar + rgbB._blue * other;
        int green = rgbA._green *scalar + rgbB._green * other;
        int red =  rgbA._red * scalar  + rgbB._red * other;

        scalar = scalar + step;
        other = 1 - scalar;
        Y = va + YW*steps;
        X = ha+ w*steps;
        SetPixel(floor(X),floor(Y),RGBVal(red,green,blue));
        steps = steps + 0.001;//s/1000; //size /1000
//        _n_horizontal
    }
}

struct Point {
   float x, y;
};
//#define size 3
using namespace std;
//function to calculate dot product of two vectors
float dot_product(Point vector_a, Point vector_b) {
   float product = 0;
   for (int i = 0; i < 2; i++)
   product = product + vector_a.x * vector_b.x;
   product = product + vector_a.y * vector_b.y;


   return product;
}

void PixelWidget::Barycentric( float ha, float va, float hb, float vb, float hc, float vc, float &u, float &v, float &w, const RGBVal& rgbA, const RGBVal& rgbB, const RGBVal& rgbC)
{
    Point p;
    ofstream myfile ("bary.txt");
    if (myfile.is_open())
    {

    ofstream ppmfile ("ppm.ppm");

    if (ppmfile.is_open()){
        ppmfile << "P3\n";
        ppmfile << "70\n";
        ppmfile << "70\n";
        ppmfile << "256\n";


    for (int x= 0; x<70; x++) {
        for (int y=0; y<70; y++){
            p.x = x;
            p.y = y;

            Point a = {ha,va};
            Point b = {hb,vb};
            Point c = {hc,vc};
            Point v0 = {b.x - a.x, b.y - b.x};
            Point v1 = {c.x - a.x, c.y - a.x};
            Point v2 = {p.x - a.x, p.y - a.x};
            //Vector v0 =
            float d00 = dot_product(v0, v0);
            float d01 = dot_product(v0, v1);
            float d11 = dot_product(v1, v1);
            float d20 = dot_product(v2, v0);
            float d21 = dot_product(v2, v1);
            float denom = d00 * d11 - d01 * d01;
            v = (d11 * d20 - d01 * d21) / denom;
            w = (d00 * d21 - d01 * d20) / denom;
            u = 1.0f - v - w;

            if ((v+u+w) > 0) {
                //Pixel colour
                RGBVal col;
                int Ablue = rgbA._blue * u ;//+ rgbB._blue * other;
                int Agreen = rgbA._green *u ;//+ rgbB._green * other;
                int Ared =  rgbA._red * u ;// + rgbB._red * other;

                int Bblue = rgbB._blue * v ;//+ rgbB._blue * other;
                int Bgreen = rgbB._green *v ;//+ rgbB._green * other;
                int Bred =  rgbB._red * v ;// + rgbB._red * other;

                int Cblue = rgbC._blue * (1-u-v) ;//+ rgbB._blue * other;
                int Cgreen = rgbC._green *(1-u-v) ;//+ rgbB._green * other;
                int Cred =  rgbC._red * (1-u-v) ;// + rgbB._red * other;
                col = RGBVal(Ared+Bred+Cred,Agreen+Bgreen+Cgreen,Ablue+Bblue+Cblue);
                SetPixel(x,y,col);
                //x,y inside
                if (col._red > 256 || col._green > 256 || col._blue > 256) {
                                    ppmfile << "0 0 0 "  ;
                                    myfile << 0 << " \n" ;


                } else {
                 ppmfile << col._red << " " << col._green << " " <<  col._blue << " ";
                myfile << 1 << " \n" ;}

            } else {
                //x,y outside
                ppmfile << "0 0 0 " ;

            }

        }
        ppmfile  << " \n" ;



      }
     ppmfile.close();
    }

        myfile.close();
      }
      else cout << "Unable to open file";
        IsInside(ha, va, hb, vb, hc, vc);



}

void PixelWidget::DrawTriangle(float ha, float va, float hb, float vb, float hc, float vc, const RGBVal& rgbA, const RGBVal& rgbB, const RGBVal& rgbC){ //height a... height b
//    DrawLine(ha,va,hc,vc,rgbB,rgbA);
//    DrawLine(hc,vc,hb,vb,rgbC,rgbB);
//    DrawLine(hb,vb,ha,va,rgbA,rgbC);
    float u,v,w;
    Barycentric(ha,va,hb,vb,hc,vc, u,  v,  w, rgbA, rgbB, rgbC);
}

float calc (float ha, float va, float hb, float vb, float hc, float vc)
{
    return (ha - hc) * (vb - vc) - (hb - hc) * (va - vc);
}

bool PointInTriangle (float ha, float va, float hb, float vb, float hc, float vc, int x, int y)
{
    float d1, d2, d3;
    bool neg, pos;

    d1 = calc(x,y, ha,va, hb,vb);
    d2 = calc(x,y, hb,vb, hc,vc);
    d3 = calc(x,y, hc,vc, ha,va);

    neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(neg && pos);
}

void PixelWidget::IsInside(float ha, float va, float hb, float vb, float hc, float vc){
    //apply to all pixels
    ofstream myfile ("results.txt");
    if (myfile.is_open()){
    for (int h =0; h < 70; h++){
        for (int v=0; v<70; v++){
            myfile << int(PointInTriangle(ha,va,hb,vb,hc,vc,h,v)) << " \n";
        }
    }
    myfile.close();
  }
  else cout << "Unable to open file";

}

//PPM Image
void PixelWidget::PPM(float ha, float va, float hb, float vb, float hc, float vc){

    ofstream myfile ("compare.txt");
    if (myfile.is_open()){
    for (int h =0; h < 70; h++){
        for (int v=0; v<70; v++){
//            if (int(PointInTriangle(ha,va,hb,vb,hc,vc,h,v)) != Barycentric( );)
//            myfile << int(PointInTriangle(ha,va,hb,vb,hc,vc,h,v)) << " \n";
        }
    }
    myfile.close();
  }
  else cout << "Unable to open file";

}

// -----------------Most code below can remain untouched -------------------------
// ------but look at where DefinePixelValues is called in paintEvent--------------

PixelWidget::PixelWidget(unsigned int n_vertical, unsigned int n_horizontal):
  _n_vertical   (n_vertical),
  _n_horizontal (n_horizontal),
  _vec_rects(0)
{
  // all pixels are initialized to black
     for (unsigned int i_col = 0; i_col < n_vertical; i_col++)
       _vec_rects.push_back(std::vector<RGBVal>(n_horizontal));

     //controls?

}



void PixelWidget::SetPixel(unsigned int i_column, unsigned int i_row, const RGBVal& rgb)
{

  // if the pixel exists, set it
  if (i_column < _n_vertical && i_row < _n_horizontal)
    _vec_rects[i_column][i_row] = rgb;
}


void PixelWidget::paintEvent( QPaintEvent * )
{

  QPainter p( this );
  // no antialiasing, want thin lines between the cell
  p.setRenderHint( QPainter::Antialiasing, false );

  // set window/viewport so that the size fits the screen, within reason
  p.setWindow(QRect(-1.,-1.,_n_vertical+2,_n_horizontal+2));
  int side = qMin(width(), height());  
  p.setViewport(0, 0, side, side);

  // black thin boundary around the cells
  QPen pen( Qt::black );
  pen.setWidth(0.);

  // here the pixel values defined by the user are set in the pixel array
  DefinePixelValues();


  DrawLine(5,5,55,55,RGBVal(255,0,0),RGBVal(0,255,255),_n_horizontal);
  DrawTriangle(0,0 ,0,30, 25,0, RGBVal(250,0,0),RGBVal(0,0,250),RGBVal(0,250,0));


  for (unsigned int i_column = 0 ; i_column < _n_vertical; i_column++)
    for(unsigned int i_row = 0; i_row < _n_horizontal; i_row++){
      QRect rect(i_column,i_row,1,1);
      QColor c = QColor(_vec_rects[i_column][i_row]._red, _vec_rects[i_column][i_row]._green, _vec_rects[i_column][i_row]._blue);
    
      // fill with color for the pixel
      p.fillRect(rect, QBrush(c));
      p.setPen(pen);
      p.drawRect(rect);
    }
}
