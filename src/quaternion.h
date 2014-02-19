class quaternion
{
    public:
        quaternion();
        quaternion(double w, double x, double y, double z);
        double theta();
        double vx();
        double vy();
        double vz();

        double getw();
        double getx();
        double gety();
        double getz();

        quaternion compose(quaternion q);
        void normalize();
        double magnitude();

        void rotate(double &vx, double &vy, double &vz);
        void conjugate();

    private:
        double w, x, y, z;
};

quaternion makeQuaternion(double theta, double x, double y, double z);
