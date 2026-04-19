#include "Renderer.h"

static FILE* Renderer_ValidateImage(FileHeader *fHead, InfoHeader *iHead)
{
    FILE *file = fopen(FILE_PATH, "rb");
    if (!file) { return NULL; }
    fread(fHead, sizeof(FileHeader), 1, file);
    fread(iHead, sizeof(InfoHeader), 1, file);

    if (fHead->Type != MAGIC_BMP 
    || iHead->BitsPerPixel != PIXEL_BIT_SIZE 
    || iHead->Compression != UNCOMPRESSED) 
    {
        fclose(file);
        return NULL;
    }

    return file;
}

void Renderer_DrawImage(Context *context, const Point center)
{
    FileHeader fHead; InfoHeader iHead; 
    FILE *file = Renderer_ValidateImage(&fHead, &iHead); 
    if(file == NULL) { return; }
    short rowWidth = iHead.Width * PIXEL_BYTE_SIZE;
    int paddingBytes = (4 - (rowWidth % 4)) % 4; 
    fseek(file, fHead.Offset, SEEK_SET);

    Point topLeft = { 
        .X = center.X - (iHead.Width / 2), 
        .Y = center.Y - (iHead.Height / 2) 
    };

    for(short y = iHead.Height - 1; y >= 0; y--)
    {
        for(short x = 0; x < iHead.Width; x++)
        {
            Channel pixel[PIXEL_BYTE_SIZE];
            fread(pixel, 1, PIXEL_BYTE_SIZE, file);

            Channel paletteIndex = (
                (((pixel[2] + FLOOR_FACTOR) / BUCKET_COUNT) * CHANNEL_SQAURE) + 
                (((pixel[1] + FLOOR_FACTOR) / BUCKET_COUNT) * CHANNEL_SIZE) + 
                (( pixel[0] + FLOOR_FACTOR) / BUCKET_COUNT)
            );

            context->DrawBuffer.Frame[y + topLeft.Y][x + topLeft.X] = paletteIndex;
        }

        if(paddingBytes > 0) { fseek(file, paddingBytes, SEEK_CUR); }
    }
    
    fclose(file);
    Renderer_RenderFrame(context);
    CommandBuffer_Flush(&context->CommandBuffer, &context->Platform);
}