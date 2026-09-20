#include <cmath>

class motor_set{
    private:
        double J;           //转动惯量
        double torque;   //控制转矩
        double Tl;      //负载力矩
        double B;       //粘性阻尼系数
        double angle;   //角度
        double velocity;//角速度
        double a;       //角加速度
        double dt;      //采样周期
        

    public:
        motor_set(double j,double tl,double b,double delta_time,double tor)
        {
            J = j;
            Tl = tl;
            B = b;
            torque = tor;
            velocity = 0;
            angle = 0;
            dt = delta_time;
        }
        void motor_update()
        {
            a = (torque - Tl - B*velocity)/J;           
            velocity += a*dt; 
            angle += velocity*dt;
            while(angle>2*M_PI)
            {
                angle -= 2*M_PI;
            }
            while(angle<0)
            {
                angle += 2*M_PI;
            }
        }

        void set_torque(double a){torque = a;}
        double get_torque()const{return torque;}
        double get_angle()const{return angle;}
        double get_velocity()const{return velocity;}
        void set_dt(double t){dt = t;}

};
