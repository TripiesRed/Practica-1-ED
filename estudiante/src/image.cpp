/**
 * @file image.cpp
 * @brief Fichero con definiciones para los métodos primitivos de la clase Image
 *
 */

#include <cstring>
#include <cassert>
#include <iostream>

#include <image.h>
#include <imageIO.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/

void Image::Allocate(int nrows, int ncols, byte * buffer){
    rows = nrows;
    cols = ncols;

    img = new byte * [rows];

    if (buffer != 0)
        img[0] = buffer;
    else
        img[0] = new byte [rows * cols];

    for (int i=1; i < rows; i++)
        img[i] = img[i-1] + cols;
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
        delete [] img[0];
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
    for (int k=0; k<rows*cols; k++) set_pixel(k,value);
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
    img[0][k] = value;
}

// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    return img[0][k];
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    byte * p = img[0];
    return WritePGMImage(file_path, p, rows, cols);
}

void Image::Invert() {
    for (int i=0; i<size(); i++)
        set_pixel(i,255-get_pixel(i));
}

Image Image::Crop(int nrow, int ncol, int height, int width) const {

    if(ncol>= get_cols() || nrow>= get_rows() || height<=0 || width<=0){
        width = 0;
        height = 0;
    }

    else{
        if(width+ncol> get_cols())
            width -= get_cols()-ncol;

        if(height+nrow> get_rows())
            height = get_rows()-nrow;
    }

    Image newimage (height, width);
    for(int i= 0; i < width; i++){

        for(int j=0; j < height; j++){

            newimage.set_pixel(j, i, get_pixel(j+nrow,i+ncol));
        }
    }

    return newimage;
}

Image Image::Zoom2X() const {

    int newheight = get_rows()*2;
    int newwidth = get_cols()*2;
    Image newimage(newheight, newwidth);
    byte valor_aux=0;

    //Asignación de pixeles

    for(int i = 0; i < newheight-1; i++){
        for(int j = 0; j < newwidth-1; j++){

            if(i%2 == 0) {
                if (j%2 == 0)
                    newimage.set_pixel(i, j, get_pixel(i/2, j/2));   //Filas y columnas pares

                else{
                    valor_aux= (get_pixel(i/2,j/2) + get_pixel(i/2,j/2+1)) /2;
                    //cout << get_pixel(i/2,j/2)(int)valor_aux << endl;
                    newimage.set_pixel(i, j, valor_aux); //Interpolación
                }
            }

            else{
                valor_aux= (get_pixel(i/2,j/2) + get_pixel(i/2+1,j/2)) /2;
                newimage.set_pixel(i, j, valor_aux); //Interpolación
            }
        }

    }

    return newimage;
}

