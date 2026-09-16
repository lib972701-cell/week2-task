#include <iostream>

class pid{
    private:
        int sample_time;      //采样周期，初始化为10ms
        double last_ve;
        double this_ve;
        double target_ve;
        double k_p;
        double k_i;
        double k_d;
        double error_sigma;
        bool status;
        
        
    public:
        double dt;
        pid(int st,double lve,double tar,double kp,double ki,double kd)
        {
            sample_time = st;
            last_ve = lve;
            this_ve = 0.0f;
            target_ve = tar;
            k_p = kp;
            k_i = ki;
            k_d = kd;
            error_sigma = 0.0f;
            status = true;
        }
         double pid_process(double ve_input,double dt)
        {
            double this_ve = ve_input;
            double result = 0;
            double error = target_ve-this_ve;  //偏差值
            error_sigma += error*dt;            //误差积分项
            double d_ve = status?0.0:(this_ve-last_ve)/dt;  //误差微分项
            result = k_p * error+k_i * error_sigma-k_d * d_ve;
            last_ve = this_ve;
            status = false;
            return result;
        }
        int getsample()const{return sample_time;}
        void set_target(double ta){target_ve = ta;}

};