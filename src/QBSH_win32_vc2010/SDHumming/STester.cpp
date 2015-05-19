/*
* STester.cpp  SDHumming Tester routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#include "SModel.h"
//#include "SSearch.h"
#ifdef WIN32
#include "SDFuzzySearch.h"
#pragma comment(lib, "SDFuzzySearch.lib")
#endif
#include "SUtil.h"
#include "SMelody.h"
#include <stdio.h>
#include <time.h>
#include<iostream>
#include <string>
#include<fstream>



/* both frame-based and note-based melody feature are extracted */
int SMelodyFeatureExtraction(char* filename, float*& pFeaBuf,int &nFeaLen, SNote *&Query, int &nNoteLen)
	//pFeaBuf,pitch ; SNote,note
{
	/* 0. pitch extraction */
	SPitchContourExtraction(filename,pFeaBuf,nFeaLen);	

	/* 1. five point median filtering */
	SMedianFilter(pFeaBuf,nFeaLen);
	if(nFeaLen<20){
		printf("Too short! please try again\n");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ERROR_CODE_TOO_SHORT_INPUT;
	}

	/* 2. post-processing the pitch sequence and re-sampling the pitch sequence */
	SProcessQuery(pFeaBuf,nFeaLen);
	if(nFeaLen<20){
		printf("Too short! please try again\n");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ERROR_CODE_TOO_SHORT_INPUT;
	}

	/* 3. note transcription */     //note segmentation
	STranscribeQueryNote(pFeaBuf,nFeaLen, Query, nNoteLen);
	if(nFeaLen<20){
		printf("Too short! please try again\n");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ERROR_CODE_TOO_SHORT_INPUT;
	}

	return 0;
}

/*search one query */
/*int STester(char* szModel, char* szModelinfo, char* szWav, char* szOut){
	int i=0;

	/*
	//0, Load Model
	SModel *SQBHModels=NULL;
	char **szModelInfoStrs=NULL; 
	int nModels=0;
	int nTotalModel=SLoadModel(szModel, SQBHModels, nModels);
	if(nTotalModel<=0){
	printf("Error on loading model!\n");
	return -1;
	}
	int nTotalSongs=SReadMelodyDBInfoFile(szModelinfo,szModelInfoStrs);
	if(nTotalModel<=0){
	printf("Error on loading modelinfo!\n");
	return -1;
	}
	

	//1, Feature Extraction
	float *pFeaBuf = NULL;
	int nFeaLen = 0;
	SNote *QueryNotes= NULL;
	int nNoteLen=0;
	SMelodyFeatureExtraction(szWav,pFeaBuf,nFeaLen,QueryNotes,nNoteLen);	   //分别提取出pitch feature :pFeaBuf 和note feature:QueryNotes

	//2, Melody Search
	NoteBasedResStru *myEMDResStru=new NoteBasedResStru[nTotalModel];
	SNoteBasedMatch(SQBHModels, nModels, QueryNotes, nNoteLen,myEMDResStru,nFeaLen);

	FrameBasedResStru *myDTWResStru=new FrameBasedResStru[20];   //取EMD结果的前20个，做DTW
	SFrameBasedMatch(SQBHModels, nModels, pFeaBuf, nFeaLen, myEMDResStru, 20, myDTWResStru);

	//3, Finalize and print the result
	FILE *OutFile=fopen(szOut,"a+t");
	fprintf(OutFile,"%s ",szWav);
	for(i=0;i<20;i++){
		fprintf(OutFile,"%d: %s, %f; ",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
		printf("%d: %s, %f\n",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
	}
	fprintf(OutFile,"\n");
	fclose(OutFile);

	for(i=0;i<nTotalSongs;i++){
		if(NULL!=SQBHModels[i].PhrasePos){
			delete[] SQBHModels[i].PhrasePos; 
			SQBHModels[i].PhrasePos=NULL;
		}
		if(NULL!=SQBHModels[i].sNotes){
			delete[] SQBHModels[i].sNotes;
			SQBHModels[i].sNotes=NULL;
		}
	}

	for(i=0;i<nTotalSongs;i++){
		if(NULL != szModelInfoStrs[i]){
			delete[] szModelInfoStrs[i];
			szModelInfoStrs[i]=NULL;
		}
	}
	if(NULL!=myEMDResStru){
		delete[] myEMDResStru;
		myEMDResStru=NULL;
	}
	if(NULL!=myDTWResStru){
		delete[] myDTWResStru;
		myDTWResStru=NULL;
	}
	if(NULL!=SQBHModels){
		delete[] SQBHModels;
		SQBHModels=NULL;
	}
	if(NULL!=szModelInfoStrs){
		delete[] szModelInfoStrs;
		szModelInfoStrs=NULL;
	}
	if(NULL!=QueryNotes){
		delete[] QueryNotes;
		QueryNotes=NULL;
	}
	if(NULL!=pFeaBuf){
		delete[] pFeaBuf;
		pFeaBuf=NULL;
	}
	return 0;
}
*/
// //////////// 原主函数
// int main(int argc, char* argv[]){
// 	if (argc != 5){ 
// 		printf("usage: SDHumming.exe QBHModel.dat QBHModel.info test.wav result.txt \n"); 
// 		return 0; 
// 	}
// 
// 	printf("%s,%s,%s,%s\n",argv[1],argv[2],argv[3],argv[4]);
// 
// 	STester(argv[1],argv[2],argv[3],argv[4]);
// 	printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
// 	return 0;
// }

///////////////////48 wav，批次处理
//
//int main(int argc, char* argv[]){
//	system("DIR /B E:\\音频检索\\GitHub下载代码\\9SD\\src\\QBSH_win32_vc2010\\SDHumming\\wav48 >wav48.list");
//	std::ifstream wavlist("wav48.list");
//	std::string str;
//	int count=0;
//	while (NULL!=getline(wavlist,str))
//	{
//		count++;
//		std::cout<<str<<std::endl;
//		char song[20];
//		strcpy(song, str.c_str());
//
//		argv[3]=new char[30];
//		strcpy(argv[3],"wav48\\");
//		strcat(argv[3],song);
//		printf("%s\n",argv[3]);
//
//		char name[10],suf[10];
//		sscanf(song,"%[^.].%[^.]",name,suf);
//		argv[4]=new char[30];
//		strcpy(argv[4],"result48_1\\");
//		strcat(argv[4],name);
//		strcat(argv[4],".txt");
//		printf("%s,%s,%s,%s\n",argv[1],argv[2],argv[3],argv[4]);
//
//		STester(argv[1],argv[2],argv[3],argv[4]);
//		printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
//	}
//	printf("%d",count);
//	return 0;
//}
////////////////all Jang's data  , 4431 query wav

//int main(int argc, char* argv[]){
//	
//	std::ifstream wavlist("MIR-corpus-wave.txt");
//	std::string str;
//	int count_qy=0;
//	while (getline(wavlist,str)!=NULL)
//	{
//		//std::cout<<str<<std::endl;
//		char song[60];
//		strcpy(song, str.c_str());
//
//		argv[3]=new char[160];
//		strcpy(argv[3],song);
//		//printf("%s\n",argv[3]);
//
//		argv[4]=new char[160];
//		strcpy(argv[4],"result_jang\\");
//		strcat(argv[4],argv[3]+45);
//	
//		argv[4][strlen(argv[4])-4]='\0';
//		strcat(argv[4],".txt");
//		//printf("%s\n",argv[4]);
//		
//		printf("%s,%s,%s,%s\n",argv[1],argv[2],argv[3],argv[4]);
//
//		STester(argv[1],argv[2],argv[3],argv[4]);
//		printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
//		count_qy++;
//		//printf("The %d song\n",count_qy);
//
//	}
//	printf("%d",count_qy);
//	return 0;
//}
///////////////////////IOACAS data,759 query wav
//need  to change arguments of cmd : model_IOACAS\QBH.Model   model_IOACAS\QBHModel.info
int main(int argc, char* argv[]){
	//0, Load Model
	SModel *SQBHModels=NULL;
	char **szModelInfoStrs=NULL; 
	int nModels=0;
	int nTotalModel=SLoadModel(argv[1], SQBHModels, nModels);
	if(nTotalModel<=0){
		printf("Error on loading model!\n");
		return -1;
	}
	int nTotalSongs=SReadMelodyDBInfoFile(argv[2],szModelInfoStrs);
	if(nTotalModel<=0){
		printf("Error on loading modelinfo!\n");
		return -1;
	}

	std::ifstream wavlist("IOACAS-wave.txt");
	std::string str;
	int count_qy=0;
	while (getline(wavlist,str)!=NULL)
	{
		char song[60];
		strcpy(song, str.c_str());

		argv[3]=new char[160];
		strcpy(argv[3],song);
		//printf("%s\n",argv[3]);

		argv[4]=new char[160];
		strcpy(argv[4],"result_IOACAS\\");
		strcat(argv[4],argv[3]+39);

		argv[4][strlen(argv[4])-4]='\0';
		strcat(argv[4],".txt");
		//printf("%s\n",argv[4]);

		//printf("%s,%s,%s,%s\n",argv[1],argv[2],argv[3],argv[4]);
		//1, Feature Extraction
		float *pFeaBuf = NULL;
		int nFeaLen = 0;
		SNote *QueryNotes= NULL;
		int nNoteLen=0;
		SMelodyFeatureExtraction(argv[3],pFeaBuf,nFeaLen,QueryNotes,nNoteLen);	   //分别提取出pitch feature :pFeaBuf 和note feature:QueryNotes

		//2, Melody Search
		NoteBasedResStru *myEMDResStru=new NoteBasedResStru[nTotalModel];
		SNoteBasedMatch(SQBHModels, nModels, QueryNotes, nNoteLen,myEMDResStru,nFeaLen);

		FrameBasedResStru *myDTWResStru=new FrameBasedResStru[20];   //取EMD结果的前20个，做DTW
		SFrameBasedMatch(SQBHModels, nModels, pFeaBuf, nFeaLen, myEMDResStru, 20, myDTWResStru);

		//3, Finalize and print the result
		FILE *OutFile=fopen(argv[4],"a+t");
		fprintf(OutFile,"%s ",argv[3]);
		for(int i=0;i<20;i++){
			fprintf(OutFile,"%d: %s, %f\n; ",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
			
			printf("%d: %s, %f\n",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
		}
		fprintf(OutFile,"Elapsed time: %f ",clock()/(CLOCKS_PER_SEC+0.0f));
		fprintf(OutFile,"\n");
		fclose(OutFile);

		if(NULL!=myEMDResStru){
			delete[] myEMDResStru;
			myEMDResStru=NULL;
		}
		if(NULL!=myDTWResStru){
			delete[] myDTWResStru;
			myDTWResStru=NULL;
		}
		if(NULL!=QueryNotes){
			delete[] QueryNotes;
			QueryNotes=NULL;
		}
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}

		//STester(argv[3],argv[4]);
		printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
		count_qy++;
		//printf("The %d song\n",count_qy);

	}
	if(NULL!=SQBHModels){
		delete[] SQBHModels;
		SQBHModels=NULL;
	}
	if(NULL!=szModelInfoStrs){
		delete[] szModelInfoStrs;
		szModelInfoStrs=NULL;
	}

	printf("%d",count_qy);
	return 0;
}