#include <iostream>

class pid{
    private:
        double sample_time;      //控制周期
        double last_ve;
        double this_ve;
        double target_ve;
        double k_p;
        double k_i;
        double k_d;
        double error_sigma;
        bool status;
        double pid_max;
        double pid_min;
        
        
    public:
        double dt;
        pid(int st,double lve,double tar,double kp,double ki,double kd,double max,double min)
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
            pid_max = max;
            pid_min = min;
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
            if(result > pid_max){result = pid_max;}
            if(result < pid_min){result = pid_min;}
            return result;
        }
        int getsample()const{return sample_time;}
        void set_target(double ta){target_ve = ta;}
};