class PID{

    private:

        float Kd;
        float Ki;
        float Kp;

        float P_pid;
        float I_pid;
        float D_pid;

        float P_err;
        float I_err;
        float D_err;

        float dt;
        float P_pid_prev;

    public:

        PID();
        
        int init();
        void setTuning(float kd, float ki, float kp);
        int calcPID(float aimVal, float inVal);
        
};
