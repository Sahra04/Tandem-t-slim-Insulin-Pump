#ifndef INSULINPUMPDEVICE_H
#define INSULINPUMPDEVICE_H


class InsulinPumpDevice
{
public:
    InsulinPumpDevice(double currentBloodGlucose);
    double getCurrentBG();
    void setCurrentBG(double currentBloodGlucose);

private:
    double currentBG;
};

#endif // INSULINPUMPDEVICE_H
