#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>

using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    bool fileMode = false;
    if(std::string(argv[3]) == "FILE")
    {
        fileMode = true;
    }
    else if (std::string(argv[3]) == "CAMERA")
    {
        fileMode = false;
    }
    else
    {
        std::cerr << "Please enter either FILE or CAMERA as your third parameter!\n";
        return 0;
    }
    
    cv::VideoCapture webcam;
    if(fileMode == false)
    {
        webcam.open(0);

        if( !webcam.isOpened() )
        {
            std::cerr << "***Could not initialize capturing...***\n";
            std::cerr << "Current parameter's value: \n";
            return -1;
        }
    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true); //vocab yaml mono true

    // Main loop
    cv::Mat im;
    cv::Mat mask;
    while(1)
    {
        if(fileMode)
        {
            //argv[4] should be real video, and argv[5] should be the human mask video
            //TODO : get something into im
            //TODO : get something into mask too
        }
        else
        {
            // im is from live camera
            webcam >> im;
            //TODO: get the mask out of im
            //TODO: how to run python from here?
        }


#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the image to the SLAM system
        SLAM.TrackMonocular(im,0); //lets look later how the system uses that "timestamp"

        //TODO : How to calibrate the camera with OpenCV? 
        //TODO : Once calibrated, test the SLAM on the plane scene, which motion can initialize the SLAM system?
        //TODO : Shoot the video and process the mask out of it.
        //TODO : Find ways to import video frames to c++

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

        // Temp info I will hardcode : bg is a plane, and human is fixed in place. Distace to a plane is known. Camera only move sideways.
        // Question : What is the unit of stored camera position data? 

        //TODO : How can I get each camera pose out of ORB_SLAM?

        //TODO : Store camera pose and position. Store all the colors of that pose into plane. Masked pixels does not count.

        //TODO : For current image, attempt to replace masked pixels with something else. Try to interpolate colors from the plane if found color data.

        //TODO : output the image to result window (not the same as ORB SLAM window)
    }

    // Stop all threads
    SLAM.Shutdown();

/*
    // Tracking time statistics
    sort(vTimesTrack.begin(),vTimesTrack.end());
    float totaltime = 0;
    for(int ni=0; ni<nImages; ni++)
    {
        totaltime+=vTimesTrack[ni];
    }
    cout << "-------" << endl << endl;
    cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;
    cout << "mean tracking time: " << totaltime/nImages << endl;

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
*/

    return 0;
}
