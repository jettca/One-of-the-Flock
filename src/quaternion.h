class quaternion
{
    public:
        quaternion(double w, double x, double y, double z);
        float theta();
        float vx();
        float vy();
        float vz();

        float getw();
        float getx();
        float gety();
        float getz();

        quaternion compose(quaternion q);

    private:
        double w, x, y, z;
};
