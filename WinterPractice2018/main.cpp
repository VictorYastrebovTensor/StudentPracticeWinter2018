#include <Windows.h>
#include <iostream>
#include <vector>

using namespace std;

typedef unsigned char BYTE;

struct mColor
{
   BYTE r;
   BYTE g;
   BYTE b;
};

class BmpReader
{
public:

    int CreateNewBmp(const std::string &file_path, int Width, int Height){
        DWORD RW;
        mColor colorGreen,colorRed,colorBlue;
        colorGreen.b = 0x00;
        colorGreen.g = 0xFF;
        colorGreen.r = 0x00;

        colorRed.b = 0x00;
        colorRed.g = 0x00;
        colorRed.r = 0xFF;

        colorBlue.b = 0xFF;
        colorBlue.g = 0x00;
        colorBlue.r = 0x00;
        memset (&bfile, 0, sizeof(bfile));
        memset (&binfo, 0, sizeof(binfo));
        bfile.bfType = 0x4D42;
        bfile.bfOffBits = sizeof(bfile) + sizeof(bfile);
        bfile.bfSize = bfile.bfOffBits +
            3 * Width * Height +
            Height * ((3 * Width) % 4);

        binfo.biSize = sizeof(binfo);
        binfo.biBitCount = 24;
        binfo.biCompression = BI_RGB;
        binfo.biHeight = Height;
        binfo.biWidth = Width;
        binfo.biPlanes = 1;



        HANDLE hFile = CreateFile (file_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
            if (hFile == INVALID_HANDLE_VALUE)
                return 1;

            WriteFile (hFile, &bfile, sizeof (bfile), &RW, NULL);
            WriteFile (hFile, &binfo, sizeof (binfo), &RW, NULL);
            for(int y = 0;y<Height;y++){
                for(int x = 0;x<Width;x++){
                    if(((x / (Width/5)) % 2 == 0) && ((y / (Height/5)) % 2 == 0))
                  setPixelColor(hFile,colorBlue);
                    else {
                        setPixelColor(hFile,colorGreen);
                    }
                }
            }
            CloseHandle(hFile);
            cout<<bfile.bfSize;
            return 0;

    }

    int LoadFromFile(const std::string &file_path)
    {


        HANDLE hFile = CreateFile (file_path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (hFile == INVALID_HANDLE_VALUE)
                return 1;



            DWORD RW;
            ReadFile(hFile,&bfile,sizeof(bfile),&RW,NULL);
            ReadFile(hFile,&binfo,sizeof(binfo),&RW,NULL);
            for(int i = 0; i < (bfile.bfSize-bfile.bfOffBits);i++){
                v_bmp_data.reserve(bfile.bfSize-bfile.bfOffBits);
               ReadFile(hFile,&v_bmp_data[i], sizeof(BYTE),&RW,NULL);
            }



        // прочитать два заголовка с жестокого диска
        // прочитать матрицу bmp файла



        return 0;
    }



    int WriteBmpToFile(const std::string &file_path)
    {
        DWORD RW;
        HANDLE hFile = CreateFile (file_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
            if (hFile == INVALID_HANDLE_VALUE)
                return 1;

            WriteFile (hFile, &bfile, sizeof (bfile), &RW, NULL);
            WriteFile (hFile, &binfo, sizeof (binfo), &RW, NULL);

            for(int i = 0; i < (bfile.bfSize-bfile.bfOffBits);i++){
               WriteFile(hFile,&v_bmp_data[i], sizeof(BYTE),&RW,NULL);
            }

        return 0;
    }

    BITMAPINFOHEADER binfo;
    BITMAPFILEHEADER bfile;
    int i;

private:
    std::vector<BYTE> v_bmp_data;

    void setPixelColor(HANDLE &hFile,mColor color){
        DWORD RW;
        WriteFile (hFile, &color.b, sizeof (color.b), &RW, NULL);
        WriteFile (hFile, &color.g, sizeof (color.g), &RW, NULL);
        WriteFile (hFile, &color.r, sizeof (color.r), &RW, NULL);

    }


};


int main()
{

    const std::string path = "c:\\Users\\levmi\\Desktop\\bbb.bmp";
    const std::string path2 = "c:\\Users\\levmi\\Desktop\\aaa.bmp";
   //cout << "Hi!" << endl;

   BmpReader bmp_reader;
  // std::vector<BYTE> v_bmp_data =
   //        bmp_reader.LoadFromFile("lena.bmp");

   bmp_reader.CreateNewBmp(path,500,500);
   bmp_reader.LoadFromFile(path);
   bmp_reader.WriteBmpToFile(path2);



   return 0;
}
