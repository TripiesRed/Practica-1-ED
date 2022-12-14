/**
 * @file image.cpp
 * @brief Fichero con definiciones para los métodos primitivos de la clase Image
 *
 */

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>

#include <image.h>
#include <imageIO.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/

void Image::Allocate(int nrows, int ncols, byte * buffer){
    rows = nrows;
    cols = ncols;

    img = new byte*[rows];

    for (int i=0; i < rows; i++){
        img[i] = new byte [cols];

        if (buffer != 0)
            for(int j = 0; j < cols; j++)
                img[i][j] = buffer[i*rows + j];
    }


}

// Función auxiliar para inicializar imágenes con valores por defecto o a partir de un buffer de datos
void Image::Initialize (int nrows, int ncols, byte * buffer){
    if ((nrows == 0) || (ncols == 0)){
        rows = cols = 0;
        img = 0;
    }
    else Allocate(nrows, ncols, buffer);
}

// Función auxiliar para copiar objetos Imagen

void Image::Copy(const Image & orig){
    Initialize(orig.rows,orig.cols);
    for (int k=0; k<rows*cols;k++)
        set_pixel(k,orig.get_pixel(k));
}

// Función auxiliar para destruir objetos Imagen
bool Image::Empty() const{
    return (rows == 0) || (cols == 0);
}

void Image::Destroy(){
    if (!Empty()){
        for (int i = 0; i < get_rows(); i++)
            delete [] img[i];
        delete [] img;
    }
}

LoadResult Image::LoadFromPGM(const char * file_path){
    if (ReadImageKind(file_path) != IMG_PGM)
        return LoadResult::NOT_PGM;

    byte * buffer = ReadPGMImage(file_path, rows, cols);
    if (!buffer)
        return LoadResult::READING_ERROR;

    Initialize(rows, cols, buffer);
    return LoadResult::SUCCESS;
}

/********************************
       FUNCIONES PÚBLICAS
********************************/

// Constructor por defecto

Image::Image(){
    Initialize();
}

// Constructores con parámetros
Image::Image (int nrows, int ncols, byte value){
    Initialize(nrows, ncols);
    for (int k=0; k<rows*cols; k++)set_pixel(k, value);
}

bool Image::Load (const char * file_path) {
    Destroy();
    return LoadFromPGM(file_path) == LoadResult::SUCCESS;
}

// Constructor de copias

Image::Image (const Image & orig){
    assert (this != &orig);
    Copy(orig);
}

// Destructor

Image::~Image(){
    Destroy();
}

// Operador de Asignación

Image & Image::operator= (const Image & orig){
    if (this != &orig){
        Destroy();
        Copy(orig);
    }
    return *this;
}

// Métodos de acceso a los campos de la clase

int Image::get_rows() const {
    return rows;
}

int Image::get_cols() const {
    return cols;
}

int Image::size() const{
    return get_rows()*get_cols();
}

// Métodos básicos de edición de imágenes
void Image::set_pixel (int i, int j, byte value) {
    img[i][j] = value;
}
byte Image::get_pixel (int i, int j) const {
    return img[i][j];
}

// This doesn't work if representation changes
void Image::set_pixel (int k, byte value) {
    int fils = k/get_cols();
    int cols = k%get_cols();
    set_pixel(fils, cols, value);
}

// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    int fils = k/get_cols();
    int cols = k%get_cols();
    return get_pixel(fils, cols);
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    int nrows = get_rows();
    int ncols = get_cols();
    byte * p = new byte [nrows*ncols];

    for (int i = 0; i < nrows; i++)
        for (int j = 0; j < ncols; j++)
            p[i*ncols + j] = get_pixel(i,j);

    return WritePGMImage(file_path, p, rows, cols);
}
// Método para obtener una imagen con la tonalidad invertida
void Image::Invert(void) {
    for (int i=0; i<size(); i++)
        set_pixel(i,255-get_pixel(i));
}
// Método para obtener una subimagen
Image Image::Crop(int nrow, int ncol, int height, int width) const {

    if(ncol>= get_cols() || nrow>= get_rows() || height<=0 || width<=0){
        width = 0;
        height = 0;
    }

    else{
        if(width+ncol> get_cols())
            width = get_cols()-ncol;

        if(height+nrow> get_rows())
            height = get_rows()-nrow;
    }

    Image newimage (height, width);
    for(int i= 0; i < height; i++){

        for(int j=0; j < width; j++){

            newimage.set_pixel(i, j, get_pixel(i+nrow,j+ncol));
        }
    }

    return newimage;
}

// Método para obtener una imagen aumentada al doble de su tamaño
Image Image::Zoom2X(void) const {

    int newheight = get_rows()*2-1;
    int newwidth = get_cols()*2-1;
    Image newimage(newheight, newwidth);
    byte valor_aux=0;

    //Asignación de pixeles

    for(int i = 0; i < newheight; i++){
        for(int j = 0; j < newwidth; j++){

            if(i%2 == 0) {
                if (j%2 == 0)
                    newimage.set_pixel(i, j, get_pixel(i/2, j/2));   //Filas y columnas pares

                else{
                    valor_aux= lround((get_pixel(i/2,j/2) + get_pixel(i/2,j/2+1))/2.0 );
                    newimage.set_pixel(i, j, valor_aux); //Interpolación
                }
            }

            else{
                if (j%2 == 0){
                    valor_aux= lround((get_pixel(i/2,j/2) + get_pixel(i/2+1,j/2))/2.0 );
                    newimage.set_pixel(i, j, valor_aux); //Interpolación
                }

                else{
                    valor_aux= lround((get_pixel(i/2,j/2) + get_pixel(i/2,j/2+1) + get_pixel(i/2+1,j/2) + get_pixel(i/2+1,j/2+1))/4.0 );
                    newimage.set_pixel(i, j, valor_aux);
                }
            }
        }

    }

    return newimage;
}

// Método para obtener una imagen con tamaño reducido
Image Image::Subsample(int factor) const {

    if (factor > (int)get_rows()) factor = get_rows();
    int newheight = lround(get_rows()/factor);
    int newwidth = lround(get_cols()/factor);
    Image newimage (newheight,newwidth);
    byte valor_aux;
    double total = 0;

    //Asignación de pixeles
    for(int i = 0; i < newheight; i++){
        for(int j = 0; j < newwidth; j++){

            total= Mean(i*factor,j*factor,factor,factor);
            valor_aux= lround(total);
            newimage.set_pixel(i,j,valor_aux);

        }

    }

    return newimage;
}

// Método para obtener una imagen con nuevo contraste
void Image::AdjustContrast(byte in1, byte in2, byte out1, byte out2) {
    double quotient1 ;
    if (in1!=0) quotient1= (double)out1 / (double)in1;
    else quotient1=0;

    double quotient2 = (double)(out2 - out1) / (double)(in2 - in1);

    double quotient3;
    if (in2!=255) quotient3= (double)(255 - out2) / (double)(255 - in2);
    else quotient3=0;

    byte new_byte=0;

    for(int k = 0; k < get_rows()*get_cols(); k++){

        if(get_pixel(k) >= 0 && get_pixel(k) < in1){
            new_byte = lround(quotient1 * get_pixel(k));
        }
        else{
            if(get_pixel(k) >= in1 && get_pixel(k) <= in2){
                new_byte = lround(out1 + (quotient2 * (get_pixel(k) - in1)));
            }
            else{
                new_byte = lround(out2 + (quotient3 * (get_pixel(k) - in2)));
            }
        }
        set_pixel(k, new_byte);

    }
}

// Método para calcular el valor medio de los píxeles de una imagen
double Image::Mean(int i, int j, int height, int width) const{
    Image frag=Crop(i, j, height, width);
    double sum = 0, niter=0;

    for(int f = 0; f < frag.get_rows(); f++){
        for (int c = 0; c < frag.get_cols(); c++){
            sum += frag.get_pixel(f, c);
            niter++;
        }
    }

    double mean = sum / niter;
    return mean;
}

// Método que baraja las filas de una imagen pseudoaleatoriamente
void Image::ShuffleRows(void) {
    const int P = 9973;
    int fils = get_rows();
    int cols = get_cols();
    byte ** newimage = new byte * [fils];
    int newfil;

    for (int i = 0; i < fils; i++){
        newfil = i*P % fils;
        newimage[i]=img[newfil];
    }

   for (int i = 0; i < fils; i++)
       img[i]=newimage[i];

}

