class point
{
    public:
        point(double x, double y, double z);
        double magnitude();
        point normalize();
        point times(double a);
        point plus(point p);
        double dot(point p);
        point cross(point p);
        void print();

        double getx();
        double gety();
        double getz();
        
    private:
        double x, y, z;
};
