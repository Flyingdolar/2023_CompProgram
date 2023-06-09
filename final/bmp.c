#include "bmp.h"
#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

sbPict **initArr(int h, int w) {
    sbPict **arr = (sbPict **)malloc(h * sizeof(sbPict *));
    for (size_t hdx = 0; hdx < (size_t)h; hdx++)
        arr[hdx] = (sbPict *)malloc(w * sizeof(sbPict));
    return arr;
}

void freeArr(sbPict **arr, int h) {
    for (size_t hdx = 0; hdx < (size_t)h; hdx++)
        free(arr[hdx]);
    free(arr);
}

void printHead(sbHead head) {
    printf("bm: %c%c\n", head.bm[0], head.bm[1]);
    printf("size: %u\n", head.size);
    printf("off: %u\n", head.off);
    printf("bSize: %u\n", head.bSize);
    printf("w: %u\n", head.w);
    printf("h: %u\n", head.h);
    printf("pl: %u\n", head.pl);
    printf("bpp: %u\n", head.bpp);
    printf("cpress: %u\n", head.cpress);
    printf("bImgSize: %u\n", head.bImgSize);
    printf("Xres: %u\n", head.Xres);
    printf("Yres: %u\n", head.Yres);
    printf("used: %u\n", head.used);
    printf("impt: %u\n", head.impt);
    return;
}

sbHead initHead(int h, int w) {
    sbHead bmpHead;
    memset(&bmpHead, 0, sizeof(bmpHead));
    bmpHead.bm[0] = 'B', bmpHead.bm[1] = 'M';
    bmpHead.size = 54 + 3 * w * h;
    bmpHead.off = 54;
    bmpHead.bSize = 40;
    bmpHead.w = w, bmpHead.h = h;
    bmpHead.pl = 1;
    bmpHead.bpp = 24;
    bmpHead.cpress = 0;
    bmpHead.bImgSize = 3 * w * h;
    bmpHead.Xres = 1, bmpHead.Yres = 1;
    bmpHead.used = 0, bmpHead.impt = 0;
    return bmpHead;
}

sbPict **BMPtoArr(FILE *bmp, sbHead *bmpHead) {
    int sizeW, sizeH;
    sbPict **arr;

    if ((fread(bmpHead, sizeof(*bmpHead), 1, bmp)) != 1) {
        PRINTE("Read BMP Header Error");
        return NULL;
    }
    sizeW = bmpHead->w, sizeH = bmpHead->h;
    arr = initArr(sizeW, sizeH);
    fseek(bmp, bmpHead->off, SEEK_SET);

    for (size_t hdx = 0; hdx < (size_t)sizeH; hdx++) {
        for (size_t wdx = 0; wdx < (size_t)sizeW; wdx++) {
            if ((fread(&(arr[hdx][wdx]), 1, 3, bmp) != 3)) {
                PRINTE("Read BMP Body Error");
                return NULL;
            }
        }
    }
    return arr;
}

int ArrtoBMP(FILE **bmp, sbPict **arr, sbHead bmpHead) {
    int sizeW = bmpHead.w, sizeH = bmpHead.h;

    if ((fwrite(&bmpHead, sizeof(bmpHead), 1, *bmp)) != 1) {
        PRINTE("Write BMP Header Error");
        return -1;
    }
    fseek(*bmp, bmpHead.off, SEEK_SET);
    for (size_t hdx = 0; hdx < (size_t)sizeH; hdx++) {
        for (size_t wdx = 0; wdx < (size_t)sizeW; wdx++) {
            if ((fwrite(&arr[hdx][wdx], 1, 3, *bmp) != 3)) {
                PRINTE("Write BMP Body Error");
                return -1;
            }
        }
    }
    return 0;
}
