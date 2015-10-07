class PID{

    private:

        double Kd;
        double Ki;
        double Kp;

        double P_pid;
        double I_pid;
        double D_pid;
        double dt;
        double P_pid_prev;
        double output;

        int throttle;

    public:

        PID();
        PID(double kd, double ki, double kp);

        int init();
        void setTuning(double kd, double ki, double  kp);
        double calcPID(double aimVal, double inVal);
        
};
