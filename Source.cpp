#include <stdio.h>
#include<iostream>
#include<dirent.h>
#include<string.h>
using namespace std;
#pragma pack(1)

struct NODE {
	unsigned char   c;      // ky tu
	int     freq;   // so lan xuat hien
	bool    used;   // danh dau node da xu ly chua
	int     nLeft;  // chi so nut con nam ben trai
	int     nRight; // chi so nut con nam ben phai
};

struct MABIT {
	char* bits;
	int     soBit;
};

const int MAX_NODE = 256 * 9;
const int MAX_BIT_LEN = 10000;
NODE    huffTree[MAX_NODE];
MABIT   bangMaBit[256];

int soNodeTrongCay = 0;

void KhoiTao()
{
	for (int i = 0; i < MAX_NODE; i++) {
		huffTree[i].c = i;
		huffTree[i].freq = 0;
		huffTree[i].used = false;
		huffTree[i].nLeft = -1;
		huffTree[i].nRight = -1;
	}
}

void ThongKeTanSoXuatHien(const char* tenFile) {
	FILE* fi = fopen(tenFile, "rb");
	unsigned char c;

	while (1) {
		fread(&c, 1, sizeof(char), fi);
		if (feof(fi)) {
			break;
		}
		huffTree[c].freq++; // Ghi chú: Thao tác này để làm gì (nghĩa là gì)?      
	}

	fclose(fi);
}

void XuatBangThongKe() {
	printf("Bang thong ke tan suat: \n");
	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0)
		{// Ghi chú: tại sao cần kiểm tra điều kiện này?
			printf("%c: %d\n", i, huffTree[i].freq);
			soNodeTrongCay++;
		}
	}
}

bool Tim2PhanTuMin(int& i, int& j, int nNode) {
	i = -1;
	j = -1;

	// tim 2 phan tu co trong so nho nhat
	for (int k = 0; k < nNode; k++)
		if (huffTree[k].used == false && huffTree[k].freq > 0) { // Ghi chú: Tại sao cần kiểm tra 2 điều kiện này
			if (i == -1) {
				i = k;
			}
			else if (j == -1) {
				j = k;
			}
			else {
				if (huffTree[i].freq > huffTree[j].freq) {
					if (huffTree[k].freq < huffTree[i].freq) {
						i = k;
					}
				}
				else {
					if (huffTree[k].freq < huffTree[j].freq) {
						j = k;
					}
				}
			}
		}
	// sap xep lai 2 phan tu de co i: phan tu co trong so nho nhat, j: phan tu co trong so nho nhi 
	// co 2 truong hop can doi i,j:
	// huffTree[i].freq > huffTree[j].freq
	// huffTree[i].freq == huffTree[j].freq va (huffTree[i].c > huffTree[j].c)
	if (i != -1 && j != -1) {
		if ((huffTree[i].freq > huffTree[j].freq) || ((huffTree[i].freq > huffTree[j].freq) && (huffTree[i].c > huffTree[j].c))) {
			int t = i;
			i = j;
			j = t;
		}
		return true;
	}
	else {
		return false;
	}
}


int TaoCayHuffman() {
	int nNode = 256;
	int i, j;
	bool timThay = false;
	while (true) {

		// Ghi chú: bước này để làm gì
		timThay = Tim2PhanTuMin(i, j, nNode);
		if (!timThay) {
			break;
		}

		// Ghi chú: bước này để làm gì
		huffTree[nNode].c = (huffTree[i].c < huffTree[j].c) ? huffTree[i].c : huffTree[j].c;
		huffTree[nNode].freq = huffTree[i].freq + huffTree[j].freq;
		huffTree[nNode].nLeft = i;
		huffTree[nNode].nRight = j;

		// Ghi chú: bước này để làm gì
		huffTree[i].used = true;
		huffTree[j].used = true;

		// Ghi chú: bước này để làm gì
		huffTree[nNode].used = false;
		nNode++;

	}
	return nNode - 1; // Ghi chú: ý nghĩa của giá trị trả về?

}

void XuatCayHuffman(int node, int tab) {
	if (node == -1) {
		return;
	}
	for (int i = 0; i < tab; i++) {
		printf("\t");
	}
	if (huffTree[node].nLeft == -1 && huffTree[node].nRight == -1) {
		printf("%c\n", huffTree[node].c);
	}
	else {
		printf("%c..\n", huffTree[node].c);
		XuatCayHuffman(huffTree[node].nLeft, tab + 1);
		XuatCayHuffman(huffTree[node].nRight, tab + 1);
	}
}

void DuyetCayHuffman(int node, char maBit[], int nMaBit) {
	if (node == -1) {
		return;
	}
	if (huffTree[node].nLeft == -1 && huffTree[node].nRight == -1) {    //Ghi chú: ý nghĩa của điều kiện kiểm tra là gì? 
		bangMaBit[node].soBit = nMaBit;
		bangMaBit[node].bits = new char[nMaBit];
		for (int i = 0; i < nMaBit; i++) {
			bangMaBit[node].bits[i] = maBit[i];
		}
		return;
	}
	else {
		//Ghi chú: ý nghĩa của 2 dòng lệnh bên dưới là gì?
		maBit[nMaBit] = 0;
		DuyetCayHuffman(huffTree[node].nLeft, maBit, nMaBit + 1);

		//Ghi chú: ý nghĩa của 2 dòng lệnh bên dưới là gì?
		maBit[nMaBit] = 1;
		DuyetCayHuffman(huffTree[node].nRight, maBit, nMaBit + 1);
	}
}

void PhatSinhMaBit(int nRoot) { // Ghi chú: ý nghĩa của tham số nRoot?
	for (int i = 0; i < 256; i++) {
		bangMaBit[i].soBit = 0;
		bangMaBit[i].bits = NULL;
	};
	char maBit[MAX_BIT_LEN / 8];
	int nMaBit = 0;

	DuyetCayHuffman(nRoot, maBit, nMaBit);
}

void XuatBangMaBit() {
	for (int i = 0; i < 256; i++)
		if (bangMaBit[i].soBit > 0) {
			printf("%c: ", i);
			for (int j = 0; j < bangMaBit[i].soBit; j++)
				printf("%d", bangMaBit[i].bits[j]);
			printf("\n");
		}
}

void XuatByte(unsigned char out, int soBitCoNghia) {
	for (int i = 7; i >= 7 - soBitCoNghia + 1; i--) {
		if ((out >> i) & 1) {// Ghi chú: Thao tác này để làm gì?
			printf("1");
		}
		else {
			printf("0");
		}
	}
	printf(" ");
}



//So node trong cay -> ki tu -> tan so -> ki tu -> tan so -> ..... day cac bit da ma hoa
void GhiBangThongKe(FILE * &fpwrite) {
	//printf("Ghi bang thong ke tan suat: \n");

	fwrite(&soNodeTrongCay, sizeof(int), 1, fpwrite);
	for (int i = 0; i < 256; i++)
	{
		if (huffTree[i].freq > 0)
		{
			char c = i;
			fwrite(&c, 1, sizeof(char), fpwrite);
			int n = huffTree[i].freq;
			fwrite(&n, 1, sizeof(int), fpwrite);
			//printf("%c: %d\n",i, huffTree[i].freq);
		}
	}

	//fclose(fpwrite);

}


void MaHoa1KyTu(FILE * &fpwrite, unsigned char c, unsigned char& out, unsigned char& viTriBit) { //Ghi chú: ý nghĩa của các tham số truyền vào?

	for (int i = 0; i < bangMaBit[c].soBit; i++) {
		if (bangMaBit[c].bits[i] == 1) {
			out = out | (1 << viTriBit); // Ghi chú: Thao tác này để làm gì?
		}
		if (viTriBit == 0) { // da du 1 byte, ghi byte do ra file
			viTriBit = 7;

			//XuatByte(out, 8);

			fwrite(&out, 1, sizeof(char), fpwrite); //Ghi ma ra file ma nhi phan theo tung block byte
			out = 0;
		}
		else {
			viTriBit--;
		}
	}
}


void NenHuffman(const char* inputFile,const char* outputFile) {

	FILE* fpwrite = fopen(outputFile, "wb");

	// BUOC 1: thong ke tan suat xuat hien cua cac ki tu
	KhoiTao();

	ThongKeTanSoXuatHien(inputFile);

	XuatBangThongKe();

	GhiBangThongKe(fpwrite); //Ghi ban thong ke

	// BUOC 2: tao cay Huffman

	int nRoot = TaoCayHuffman();

	/*XuatCayHuffman(nRoot, 0);*/

	// BUOC 3: phat sinh ma bit

	PhatSinhMaBit(nRoot);

	/*XuatBangMaBit();*/

	// BUOC 4: thay the cac ky tu bang ma bit tuong ung
	unsigned char out = 0;              // ky tu se xuat ra
	unsigned char soBitCoNghia = 0;     // byte cuoi co the ko su dung het cac bit nen can luu so bit co nghia cua byte cuoi

	unsigned char c;
	unsigned char viTriBit = 7;         //Ghi chú: ý nghĩa của biến viTriBit?
	FILE* fi = fopen(inputFile, "rb");
	while (true) {
		fread(&c, 1, sizeof(char), fi);
		if (feof(fi)) {
			break;
		}
		MaHoa1KyTu(fpwrite, c, out, viTriBit);
		//fwrite(&out, 1, sizeof(char), fpwrite); //Ghi ma ra file ma nhi phan theo tung block byte
	}
	soBitCoNghia = 7 - viTriBit;
	if (soBitCoNghia == 0) {
		soBitCoNghia = 8;
	}
	else {
		//XuatByte(out, soBitCoNghia);
		fwrite(&out, 1, sizeof(char), fpwrite); //Ghi ma ra file ma nhi phan theo tung block byte
	}
	fclose(fi);
	fclose(fpwrite);
}

//Decompress:
void LoadHuffmanTree(FILE * &fpread)
{
	KhoiTao(); //Reset lai cay Huffman va load lai
	unsigned char c;
	unsigned int n;

	int soNode;
	fread(&soNode, sizeof(int), 1, fpread);

	for (int i = 0; i < soNode; i++)
	{
		fread(&c, sizeof(char), 1, fpread);
		fread(&n, sizeof(int), 1, fpread);
		huffTree[c].freq = n;
	}
}

int DemSoKiTu()
{
	int sum = 0;
	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0) {// Ghi chú: tại sao cần kiểm tra điều kiện này?
			//printf("%c: %d\n",i, huffTree[i].freq);
			sum += huffTree[i].freq;
		}
	}
	return sum;
}

int getbit(unsigned char n, int i)
{
	return (n >> i) & 0x1;
}

void onbit(char unsigned& n, int i) //Ham bat bit
{
	n = n | (0x1 << i);
}

void DeCompress(int nRoot, FILE * &fpread)
{

	FILE* fdecode = fopen("decode.txt", "wb");

	int nCurrent = nRoot;
	int soKiTu = DemSoKiTu(); //Dem so ki tu de ko bi du bit (vi doc theo bloc 8 bit)
	int isoKiTu = 0;
	//Doc block 8 bit:
	unsigned char c;

	while (1) {
		fread(&c, sizeof(char), 1, fpread);
		for (int i = 7; i >= 0; i--)
		{
			int ibit = getbit(c, i);
			//printf("%d", ibit);
			if (ibit == 1)
				nCurrent = huffTree[nCurrent].nRight;
			else
				nCurrent = huffTree[nCurrent].nLeft;

			if (huffTree[nCurrent].nLeft == -1 && huffTree[nCurrent].nRight == -1)
			{

				fwrite(&huffTree[nCurrent].c, sizeof(char), 1, fdecode);



				isoKiTu++;
				if (isoKiTu == soKiTu)
				{
					fclose(fdecode);
					return;
				}


				nCurrent = nRoot;
			}

		}

		if (feof(fpread)) {
			break;
		}
	}

	fclose(fdecode);
}

void main() {
	// nen thu muc
	//NenHuffman("input.txt","output.txt");

	//printf("______________________________\nThao tac giai nen: ");

	//KhoiTao(); //Reset lai cau Huffman

	////Doc file da nen va giai nen:
	//FILE* fpread = fopen("output.txt", "rb");
	//LoadHuffmanTree(fpread);
	//XuatBangThongKe();
	//int nRoot = TaoCayHuffman(); //Tao cay huffman tu du lieu vua load
	//XuatCayHuffman(nRoot, 0);
	//PhatSinhMaBit(nRoot);
	//XuatBangMaBit();
	//DeCompress(nRoot, fpread);


	//Nen Folder

	DIR* pDIR;
	struct dirent* entry;
	if (pDIR = opendir("test"))
	{
		while (entry = readdir(pDIR)) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				cout << entry->d_name << "\n";
				char  a[11] = { 't' ,  'e' ,  's' ,  't' ,  '/' ,  '\0' };
				strcat(a, entry->d_name);
				NenHuffman(a,"output.txt");
			}
		}
		closedir(pDIR);
	}
	/*fclose(fpread);*/

}