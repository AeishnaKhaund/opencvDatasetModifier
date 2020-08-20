//created by harshvardhan
#include "imageOverlayAndAugmentor.h"

#if __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif


void imageOverlayAndAugmentor::imageController(std::string& analysisCustom,std::string& analysis,const long  analysisLineNumbers,const long analysisCustomLineNumbers)
{
   

   cv::Mat customImage,datasetImage,image;

   std::vector<long int> N1;

    for(long i=0;i<analysisLineNumbers;i++)
        N1.push_back(0);
    
    //minimum number of readymade_images per custom image
    long int imgRatio;
    long int num;
    if(analysisLineNumbers%analysisCustomLineNumbers == 0)
    {
        imgRatio = analysisLineNumbers/analysisCustomLineNumbers;
    }
    else
    {
        imgRatio = analysisLineNumbers/analysisCustomLineNumbers +1 ;
    }

    

    //attaching to os processes
    std::random_device rd;
    std::random_device rd2;
    std::random_device rd3;
    std::random_device rd4;
    std::random_device rd5;
    std::random_device rd6;

    std::mt19937 mt(rd());
    std::mt19937 mt2(rd2());
    std::mt19937 mt3(rd3());
    std::mt19937 mt4(rd4());
    std::mt19937 mt5(rd5());
    std::mt19937 mt6(rd6());

    std::uniform_int_distribution<std::mt19937::result_type> x(0,1920);
    std::uniform_int_distribution<std::mt19937::result_type> y(0,1080);
    std::uniform_int_distribution<std::mt19937::result_type> alpha(1,3);
    std::uniform_int_distribution<std::mt19937::result_type> beta(0,100);
    std::uniform_int_distribution<std::mt19937::result_type> n1(0,analysisLineNumbers - 1);
    std::uniform_int_distribution<std::mt19937::result_type> n2(0,analysisCustomLineNumbers);
    std::uniform_int_distribution<std::mt19937::result_type> imageAdjuster(0,10);
    std::uniform_int_distribution<std::mt19937::result_type> randomer(0, 1000);
    std::uniform_int_distribution<std::mt19937::result_type> noise_stddev(0, 30);
    num = 0;
   


    for(int i=1;i<=analysisCustomLineNumbers;i++)
    {
        cv::Mat datasetImage1;
        std::ifstream csvReaderCustom(analysisCustom);
        std::string wordCustom,lineCustom;
        std::vector<std::string> rowCustom;
        lineCustom.reserve(300);

        for(int j=0;j<i-1;j++)
            std::getline(csvReaderCustom,lineCustom);
        rowCustom.clear();

        std::getline(csvReaderCustom,lineCustom);
        std::stringstream sCustom(lineCustom);
        while(std::getline(sCustom,wordCustom,','))
        {
            rowCustom.push_back(wordCustom);
        }
       
        customImage = cv::imread(rowCustom[1],cv::IMREAD_COLOR);
        
      

        //selecting a random analysiscustomlinenumbers
        

        for(long int k=0;k<imgRatio;k++)
        { 
            std::ifstream csvReader(analysis);
            std::string word, line;
            std::vector<std::string> row;
            line.reserve(300);
            row.clear();
            while(N1[num] == 1)
            {
                if (num < analysisLineNumbers)
                {
                    num = n1(mt6);
                }
            }

            N1[num] = 1;    
            for(long int a=0;a<num-1;a++)
                std::getline(csvReader,line);
            row.clear();
            std::getline(csvReader,line);
            std::stringstream s(line);
            while(std::getline(s,word,','))
            {
                row.push_back(word);
            }
           
    
            datasetImage = cv::imread(row[1],cv::IMREAD_COLOR);
         
            if((int)imageAdjuster(mt4)>=6)
            {
            datasetImage1 = imageBrightnessAndContrastControl(datasetImage,(int)beta(mt4),(int)alpha(mt5));
            }
            else
            {
                datasetImage1 = GaussianNoise(datasetImage, 0.0, (int)noise_stddev(mt4));
                datasetImage1 = ColorJitter(datasetImage1, 20.0);
            }
            std::string Name = std::to_string(num) + "customImages_" + std::to_string(i);
            std::string imgName = Name + ".jpeg";
            if ((int)randomer(mt3) <= 450)
            {
                image = imageOverlay(customImage, datasetImage1, (int)x(mt3), (int)y(mt4), Name);
            }
            /*else
            { 
                image = imageRotater(customImage, datasetImage1, (int)x(mt3), (int)y(mt5), Name);
            }*/

            imageSaver(imgName, image);
            
        }
        //closing csvReader Custom
        
    }
}

cv::Mat imageOverlayAndAugmentor::imageBrightnessAndContrastControl(cv::Mat image, int brightnessRandom,int contrastRandom) 
{
    cv::Mat modifiedImg = cv::Mat::zeros(image.size(),image.type());
    for(int y =0;y<image.rows;y++)
    {
        for(int x=0;x<image.cols;x++)
        {
            for(int c=0;c<image.channels();c++)
            {
                modifiedImg.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(contrastRandom*image.at<cv::Vec3b>(y,x)[c]+brightnessRandom);
            }
        }
    }
    
    return modifiedImg;
}


cv::Mat imageOverlayAndAugmentor::GaussianNoise(const cv::Mat src, double Mean, double StdDev)
{
    cv::Mat src_16SC, dst;
    cv::Mat Gauss_noise = cv::Mat(src.size(), CV_16SC3);
    cv::randn(Gauss_noise, cv::Scalar::all(Mean), cv::Scalar::all(StdDev));

    src.convertTo(src_16SC, CV_16SC3);
    cv::addWeighted(src_16SC, 1.0, Gauss_noise, 1.0, 0.0, src_16SC);
    src_16SC.convertTo(dst, src.type());
    return dst;
}

  cv::Mat imageOverlayAndAugmentor::ColorJitter(const cv::Mat src, double maxjit)
  {
//    cv::Mat img, dst;
//    cv::cvtColor(src, img, cv::COLOR_BGR2HSV);
//
//    std::random_device rd;
//    std::mt19937 mt(rd());
//
//    std::uniform_int_distribution<std::mt19937::result_type> jitter(0,maxjit);
//
//    for (int c = 0; c<img.channels(); c++)
//    {
//        double jit = (int)jitter(mt);
//        
//        for (int i=0; i<img.rows; i++)
//        {
//            for (int j=0; j<img.cols; j++)
//            {
//                img.at<cv::Vec3b>(i,j)[c] = cv::saturate_cast<uchar>(img.at<cv::Vec3b>(i,j)[c]+ jit);
//            }
//        }
//    }
//
//    cv::cvtColor(img, dst, cv::COLOR_HSV2BGR);
//
//    return dst;
      return src;
  }

cv::Mat imageOverlayAndAugmentor::imageOverlay(cv::Mat customImage, cv::Mat datasetImage,int x,int y,std::string imgName) 
{
    
    if(1920>(int)customImage.cols+x)
    {
        if(1080 > (int)customImage.rows+y)
        {
            x=x;
            y=y;
        }
        else
        {
            y=1080-(int)customImage.rows;   
        }
        
    }
    else
    {
        if(1080 > (int)customImage.rows + y)
        {
            x=1920-(int)customImage.cols;

        }
        else
        {
            x=1920-(int)customImage.cols;
            y=1080-(int)customImage.rows;  

        }
        
    }

    cv::Mat croppedImage;
    croppedImage = datasetImage(cv::Rect(x,y,customImage.cols,customImage.rows));
    customImage.copyTo(croppedImage);
    std::string bboxName = imgName + ".txt";
    boundingBox(x, y, customImage.rows, customImage.cols,bboxName);
    return datasetImage;
}



cv::Mat imageOverlayAndAugmentor::imageRotater(cv::Mat customImage,cv::Mat randomImage,int X,int Y,std::string imageName)
{
    bool negative = false;
    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution<std::mt19937::result_type> Angle(0,30);
    double angle = (int)Angle(mt);
    int x = X;
    int y = Y;

    double angleRad = (angle / 180) * 3.14;
    //bounding box calculations

    //rotating the image 
    cv::Point2f center(((double)customImage.cols - 1) / 2.0, ((double)customImage.rows - 1) / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle,1.0);
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), customImage.size(), angle).boundingRect2f();
    rot.at<double>(0, 2) += bbox.width / 2.0 - customImage.cols / 2.0;
    rot.at<double>(1, 2) += bbox.height / 2.0 - customImage.rows / 2.0;
    cv::Mat dst;
    cv::warpAffine(customImage, dst, rot, bbox.size());
    int height = dst.rows;
    int width = dst.cols;
    //cv::imwrite("test.png", dst);
    //cv::waitKey(2);

    if (1920 > dst.cols + x)
    {
        if (1080 > dst.rows + y)
        {
            x = x;
            y = y;
        }
        else
        {
            y = 1080 - dst.rows;
        }

    }
    else
    {
        if (1080 > dst.rows + y)
        {
            x = 1920 - dst.cols;

        }
        else
        {
            x = 1920 - dst.cols;
            y = 1080 - dst.rows;

        }

    }

    cv::Mat image = randomImage;
    cv::Mat croppedImage;
    croppedImage = image(cv::Rect(x, y, dst.cols, dst.rows));
    dst.copyTo(croppedImage);
    std::string bboxName = imageName + ".txt";
    boundingBox(x, y, width, height, bboxName);
    return dst;
}



void imageOverlayAndAugmentor::boundingBox(int x, int y, int h, int w, std::string name)
{
    double xmid = ((double)x + (double)(h / 2))/1920;
    double ymid = ((double)y + (double)(w / 2))/1080;
    double H = (double)h / 1080;
    double W = (double) w / 1920;
    std::string bboxName;
    if (fs::exists("bbox"))
    {
    	#ifdef __linux__
    		bboxName = "bbox/"+name;
    	#elif _WIN32
        	bboxName = "bbox\\" + name;
        #endif

    }
    else
    {
        fs::create_directory("bbox");
	#ifdef __linux__
    		bboxName = "bbox/"+name;
    	#elif _WIN32
        	bboxName = "bbox\\" + name;
        #endif
    }
    std::fstream fout;
    fout.open(bboxName, std::ios::out);
    fout <<0<<" "<< xmid << " " << ymid << " " << W << " " << H;
    fout.close();
}

void imageOverlayAndAugmentor::imageSaver(std::string imgName,cv::Mat image)
{
    std::string imageName;
    if (fs::exists("image"))
    {
    	#ifdef __linux__
    		imageName="image/"+imgName;
    	#elif _WIN32
        	imageName = "image\\" + imgName;
        #endif
    }
    else
    {
        fs::create_directory("image");
        #ifdef __linux__
    		imageName="image/"+imgName;
    	#elif _WIN32
        	imageName = "image\\" + imgName;
        #endif
    }
    cv::imwrite(imageName, image);
    
}
