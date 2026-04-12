namespace MotorData
{
    struct Param
    {
        double R; // resistance
        double L; // inductance
        double Kt; // torque constant
        double Ke; // electrical constant
        double J; // moment of inertia
        double b; // viscosity coefficient
    };

    struct Specs
    {
        double angleMax;
        double speedMax;
        double accelerationMax;
        double torqueMax;
    };
}

class Motor
{
public:
    Motor(const std::string& name, uint16_t id, double R, double L, double Kt, double Ke, double J, double b, double angleMax, double speedMax, double accelerationMax, double torqueMax) :
        name(name),
        id(id),
        param.R(R),
        param.L(L),
        param.Kt(Kt),
        param.Ke(Ke),
        param.J(J),
        param.b(b),
        specs.angleMax(angleMax),
        specs.speedMax(speedMax),
        specs.accelerationMax(accelerationMax),
        specs.torqueMax(torqueMax)
    {
    }

    ~Motor()
    {
    }

    void setPosition()
    {
        // set position command
    }

    void setVelocity()
    {
        // set velocity command
    }

    void enable()
    {
        // enable motor
    }

    void disable()
    {
        // disable motor
    }

private:
    std::string name;
    uint16_t id;
    MotorData::Param param;
    MotorData::Specs specs;
};


<motorData>
    <motor name="maxonMotor" id=32>
        <parameters>
            <R>1</R>
            <L>11</L>
            <Kt>111</Kt>
            <Ke>1111</Ke>
            <J>11111</J>
            <b>111111</b>
        </parameters>
        <specifications>
            <angleMax>2</angleMax>
            <speedMax>22</speedMax>
            <accelerationMax>222</accelerationMax>
            <torqueMax>2222</torqueMax>
        </specifications>
    </motor>
    <motor name="faulhaberMotor" id=35>
        <parameters>
            <R>3</R>
            <L>33</L>
            <Kt>333</Kt>
            <Ke>3333</Ke>
            <J>33333</J>
            <b>333333</b>
        </parameters>
        <specifications>
            <angleMax>4</angleMax>
            <speedMax>44</speedMax>
            <accelerationMax>444</accelerationMax>
            <torqueMax>4444</torqueMax>
        </specifications>
    </motor>
</motorData>