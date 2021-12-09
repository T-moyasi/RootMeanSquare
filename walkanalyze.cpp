#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "fileio.hpp"

using std::vector;
using std::string;
using File::FileIO;
vector<float>	Input(float acc_axis_z,float zmean ,float lpf)
{
	#define WA_STATE0			(0)
	#define WA_STATE1			(1)
	#define WA_STATE2			(2)
	#define WA_STATE3			(3)
	#define STRIDE_THRESH		(250.0f)
	#define RIGHT_ANGLE_THRESH	(-5.0f)
	#define LEFT_ANGLE_THRESH	(5.0f)
	#define	ACC_HIGH			(500)
	#define	ACC_LOW				(-500)
	#define ACC_STOP_RANGE		(500)
	#define ACC_STOP_CNT		(150)

	float	acc_diff = 0;
	static int sStateLatCnt_HIGH = 0;
	static int sStateLatCnt_LOW = 0;
	static float sAcc_Max = 0;
	static float sAcc_Min = 0;
	static unsigned long sAcc_Max_Time = 0;
	static unsigned long sAcc_Min_Time = 0;
	static unsigned long sAcc_Pre_Time = 0;
	static long long int mStepTimeCounter =0;
	static int mState =0;
	static int mStepCount =0;
	static float RMSinst=0;
	static vector<float> ans;
	static float rms =0;
	static float count =0;
	mStepTimeCounter++;

	RMSinst+=powf(acc_axis_z - zmean,2);
	switch(mState){
		case WA_STATE0:
		{
			sAcc_Max = acc_axis_z;
			sAcc_Min = acc_axis_z;
			sAcc_Max_Time = 0;
			sAcc_Min_Time = mStepTimeCounter;
			sAcc_Pre_Time = mStepTimeCounter;
			sStateLatCnt_HIGH = 0;
			sStateLatCnt_LOW = 0;
			mState = WA_STATE1;
		}
		break;

		case WA_STATE1:
		{
			acc_diff = lpf - zmean;
			
		
			if(acc_diff < ACC_HIGH){
				sStateLatCnt_HIGH++;
				if(sStateLatCnt_HIGH > 25){
					mState = WA_STATE2;
					sStateLatCnt_HIGH = 0;
				}
			}
			else{
				sStateLatCnt_HIGH = 0;
				if(acc_axis_z > sAcc_Max){
					sAcc_Max = acc_axis_z;
					sAcc_Max_Time = mStepTimeCounter;
				}
			}
		}
		break;
		case WA_STATE2:
		{
			acc_diff = lpf - zmean;
			
			if(acc_diff > (ACC_HIGH)){
				sStateLatCnt_LOW++;
				if(sStateLatCnt_LOW > 25){
					if( sAcc_Min < (zmean-500)){
						mState = WA_STATE1;
						mStepCount++;  // 歩数確定
						rms=sqrt(RMSinst/(sAcc_Min_Time - sAcc_Pre_Time));
						RMSinst=0;
						sAcc_Pre_Time = sAcc_Min_Time;
						sAcc_Min = zmean;
						sStateLatCnt_LOW = 0;
					}
					else{
						sStateLatCnt_LOW = 0;
						if(acc_axis_z < sAcc_Min){
							sAcc_Min = acc_axis_z;
							sAcc_Min_Time = mStepTimeCounter;
						}	
					}
				}
			}
			else{
				sStateLatCnt_LOW = 0;
				if(acc_axis_z < sAcc_Min){
					sAcc_Min = acc_axis_z;
					sAcc_Min_Time = mStepTimeCounter;
				}
			}

		}
		break;

		default:
		break;
	}
	ans.push_back(count*0.002);
	ans.push_back(rms);
	return ans;
}
//歩行データリストからRMSを求めてテキストとして吐き出す
//歩行データリストは.csvで生データ,1000幅の移動平均,ロウパスフィルタ;
//RMSテキストは.csvで時間t,RMSの並び
int main(){
	vector<vector<string>> walkingdata;
	vector<vector<float>> rmsdata;
	string filename="namae";
	FileIO fileio;
	//walkingdata=fileio.readText(filename);
	for_each(walkingdata.begin(),walkingdata.end(),
		[&rmsdata](vector<string> x){
			rmsdata.push_back(Input(stof(x.at(0)),stof(x.at(1)),stof(x.at(2))));
		}
	);
	//filewrite();
	return 0;
}