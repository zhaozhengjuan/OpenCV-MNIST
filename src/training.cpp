#include "training.h"

Training::Training()
{

}
//读取图像数据集
Mat Training::readImages(int opt)
{
    ifstream file;
    Mat img;
    if(opt == 0){
        cout << "\n Training...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/train-images.idx3-ubyte", ios::binary);
    }
    else{
        cout << "\n Test...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/t10k-images.idx3-ubyte", ios::binary);
    }
    // check file
    if (!file.is_open()){
        cout << "\n File Not Found!";
        return img;
    }
    /*
     byte 0 - 3 : Magic Number(Not to be used)
     byte 4 - 7 : Total number of images in the dataset
     byte 8 - 11 : rows of each image in the dataset
     byte 12 - 15 : cols of each image in the dataset
    */
    int magic_number = 0;
    int number_of_images = 0;
    int height = 0;
    int width = 0;
    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);
    file.read((char*)&number_of_images, sizeof(number_of_images));
    number_of_images = reverseDigit(number_of_images);
    std::cout<<"Number of images is: "<<number_of_images<<std::endl;
    file.read((char*)&height, sizeof(height));
    height = reverseDigit(height);
    file.read((char*)&width, sizeof(width));
    width = reverseDigit(width);
    Mat train_images = Mat(number_of_images, height*width, CV_8UC1);
    cout << "\n No. of images:" << number_of_images <<endl;
    Mat digitImg = Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < number_of_images; i++) {
        int index = 0;
        for (int r = 0; r<height; ++r) {
           for (int c = 0; c<width; ++c) {
               unsigned char temp = 0;
               file.read((char*)&temp, sizeof(temp));
               index = r*width + c;
               train_images.at<uchar>(i, index) = (int)temp;
               digitImg.at<uchar>(r, c) = (int)temp;
           }
       }
       if (i < 100) {
            imwrite(format("/home/jackey/GitHub/OpenCV-MNIST/data/images/digit_%d.png", i), digitImg);
       }
    }
    train_images.convertTo(train_images, CV_32FC1);
    return train_images;
}
//读取标记数据集
Mat Training::readLabels(int opt)
{
    ifstream file;
    Mat img;
    if(opt == 0){
        cout << "\n Training...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/train-labels.idx1-ubyte");
    }
    else{
        cout << "\n Test...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/t10k-labels.idx1-ubyte");
    }
     // check file
    if (!file.is_open()){
        cout << "\n File Not Found!";
        return img;
    }
    /*
     byte 0 - 3 : Magic Number(Not to be used)
     byte 4 - 7 : Total number of labels in the dataset
    */
    int magic_number = 0;
    int number_of_labels = 0;
    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);
    file.read((char*)&number_of_labels, sizeof(number_of_labels));
    number_of_labels = reverseDigit(number_of_labels);
    cout << "\n No. of labels:" << number_of_labels << endl;
    Mat labels = Mat(number_of_labels, 1, CV_8UC1);
    for (long int i = 0; i<number_of_labels; ++i){
        unsigned char temp = 0;
        file.read((char*)&temp, sizeof(temp));
        //printf("temp : %d\n ", temp);
        labels.at<uchar>(i, 0) = temp;
    }
    labels.convertTo(labels, CV_32SC1);
    return labels;
}

Mat Training::read_mnist_image(const string fileName)
{
    int magic_number = 0;
     int number_of_images = 0;
     int n_rows = 0;
     int n_cols = 0;

     Mat DataMat;

     ifstream file(fileName, ios::binary);
     if (file.is_open())
     {
         cout << "成功打开图像集 ... \n";

         file.read((char*)&magic_number, sizeof(magic_number));
         file.read((char*)&number_of_images, sizeof(number_of_images));
         file.read((char*)&n_rows, sizeof(n_rows));
         file.read((char*)&n_cols, sizeof(n_cols));
         //cout << magic_number << " " << number_of_images << " " << n_rows << " " << n_cols << endl;

         magic_number = reverseDigit(magic_number);
         number_of_images = reverseDigit(number_of_images);
         n_rows = reverseDigit(n_rows);
         n_cols = reverseDigit(n_cols);
         cout << "MAGIC NUMBER = " << magic_number
             << " ;NUMBER OF IMAGES = " << number_of_images
             << " ; NUMBER OF ROWS = " << n_rows
             << " ; NUMBER OF COLS = " << n_cols << endl;

         //-test-
         //number_of_images = testNum;
         //输出第一张和最后一张图，检测读取数据无误
         Mat s = Mat::zeros(n_rows, n_rows * n_cols, CV_32FC1);
         Mat e = Mat::zeros(n_rows, n_rows * n_cols, CV_32FC1);

         cout << "开始读取Image数据......\n";
         start_time_ = clock();
         DataMat = Mat::zeros(number_of_images, n_rows * n_cols, CV_32FC1);
         for (int i = 0; i < number_of_images; i++) {
             for (int j = 0; j < n_rows * n_cols; j++) {
                 unsigned char temp = 0;
                 file.read((char*)&temp, sizeof(temp));
                 float pixel_value = float((temp + 0.0) / 255.0);
                 DataMat.at<float>(i, j) = pixel_value;

                 //打印第一张和最后一张图像数据
                 if (i == 0) {
                     s.at<float>(j / n_cols, j % n_cols) = pixel_value;
                 }
                 else if (i == number_of_images - 1) {
                     e.at<float>(j / n_cols, j % n_cols) = pixel_value;
                 }
             }
         }
         end_time_ = clock();
         cost_time_ = (end_time_ - start_time_) / CLOCKS_PER_SEC;
         cout << "读取Image数据完毕......" << cost_time_ << "s\n";

         imshow("first image", s);
         imshow("last image", e);
         waitKey(0);
     }
     file.close();
     return DataMat;
}

Mat Training::read_mnist_label(const string fileName)
{
    int magic_number;
     int number_of_items;

     Mat LabelMat;

     ifstream file(fileName, ios::binary);
     if (file.is_open())
     {
         cout << "成功打开Label集 ... \n";

         file.read((char*)&magic_number, sizeof(magic_number));
         file.read((char*)&number_of_items, sizeof(number_of_items));
         magic_number = reverseDigit(magic_number);
         number_of_items = reverseDigit(number_of_items);

         cout << "MAGIC NUMBER = " << magic_number << "  ; NUMBER OF ITEMS = " << number_of_items << endl;

         //-test-
         //number_of_items = testNum;
         //记录第一个label和最后一个label
         unsigned int s = 0, e = 0;

         cout << "开始读取Label数据......\n";
         start_time_ = clock();
         LabelMat = Mat::zeros(number_of_items, 1, CV_32SC1);
         for (int i = 0; i < number_of_items; i++) {
             unsigned char temp = 0;
             file.read((char*)&temp, sizeof(temp));
             LabelMat.at<unsigned int>(i, 0) = (unsigned int)temp;

             //打印第一个和最后一个label
             if (i == 0) s = (unsigned int)temp;
             else if (i == number_of_items - 1) e = (unsigned int)temp;
         }
         end_time_ = clock();
         cost_time_ = (end_time_ - start_time_) / CLOCKS_PER_SEC;
         cout << "读取Label数据完毕......" << cost_time_ << "s\n";

         cout << "first label = " << s << endl;
         cout << "last label = " << e << endl;
     }
     file.close();
     return LabelMat;
}

/*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们
  率
  KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
*/
void Training::knnTrain()
{
    Mat train_images = readImages(0);
    Mat train_labels = readLabels(0);
    printf("\n read mnist train dataset successfully...\n");
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    knn->setDefaultK(5);
    knn->setIsClassifier(true);
    Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
    knn->train(tdata);
    knn->save("/home/jackey/GitHub/OpenCV-MNIST/data/knn_knowledge.yml");
}

void Training::testMnist()
{
    Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>("D:/vcprojects/images/mnist/knn_knowledge.yml"); // SVM-POLY - 98%
    //Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("/home/jackey/GitHub/OpenCV-MNIST/data/knn_knowledge.yml"); // KNN - 97%
    Mat train_images = readImages(1);
    Mat train_labels = readLabels(1);
    printf("\n read mnist test dataset successfully...\n");
    float total = train_images.rows;
    float correct = 0;
    Rect rect;
    rect.x = 0;
    rect.height = 1;
    rect.width = (28 * 28);
    for(int i = 0; i < total; i++){
        int actual = train_labels.at<int>(i);
        rect.y = i;
        Mat oneImage = train_images(rect);
        int digit = svm->predict(oneImage);
//        Mat result;
//        float predicted = knn->predict(oneImage, result);
//        int digit = static_cast<int>(predicted);
        if(digit == actual){
            correct++;
        }
    }
    printf("\n recognize rate : %.2f \n", correct / total);
}

void Training::testMnistSVM()
{
    //--------------------- 1. Set up training data ---------------------------------------
    Mat trainData;
    Mat labels;
    trainData = read_mnist_image(trainImage);
    labels = read_mnist_label(trainLabel);

    cout << trainData.rows << " " << trainData.cols << endl;
    cout << labels.rows << " " << labels.cols << endl;

    //------------------------ 2. Set up the support vector machines parameters --------------------
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::RBF);
    //svm->setDegree(10.0);
    svm->setGamma(0.01);
    //svm->setCoef0(1.0);
    svm->setC(10.0);
    //svm->setNu(0.5);
    //svm->setP(0.1);
    //svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON));
    svm->setTermCriteria(TermCriteria(cv::TermCriteria::Type::EPS,1000,FLT_EPSILON));
    //------------------------ 3. Train the svm ----------------------------------------------------
    cout << "Starting training process" << endl;
    start_time_ = clock();
    svm->train(trainData, ROW_SAMPLE, labels);
    end_time_ = clock();
    cost_time_ = (end_time_ - start_time_) / CLOCKS_PER_SEC;
    cout << "Finished training process...cost " << cost_time_ << " seconds..." << endl;

    //------------------------ 4. save the svm ----------------------------------------------------
    svm->save("mnist_dataset/mnist_svm.xml");
    cout << "save as /mnist_dataset/mnist_svm.xml" << endl;


    //------------------------ 5. load the svm ----------------------------------------------------
    cout << "开始导入SVM文件...\n";
    Ptr<SVM> svm1 = StatModel::load<SVM>("mnist_dataset/mnist_svm.xml");
    cout << "成功导入SVM文件...\n";


    //------------------------ 6. read the test dataset -------------------------------------------
    cout << "开始导入测试数据...\n";
    Mat testData;
    Mat tLabel;
    testData = read_mnist_image(testImage);
    tLabel = read_mnist_label(testLabel);
    cout << "成功导入测试数据！！！\n";


    float count = 0;
    for (int i = 0; i < testData.rows; i++) {
        Mat sample = testData.row(i);
        float res = svm1->predict(sample);
        res = std::abs(res - tLabel.at<unsigned int>(i, 0)) <= FLT_EPSILON ? 1.f : 0.f;
        count += res;
    }
    cout << "正确的识别个数 count = " << count << endl;
    cout << "错误率为..." << (10000 - count + 0.0) / 10000 * 100.0 << "%....\n";

}

int Training::reverseDigit(int num)
{
    unsigned char c1,c2,c3,c4;

    c1=num&255;
    c2=(num>>8)&255;
    c3=(num>>16)&255;
    c4=(num>>24)&255;

    return ((int)c1<<24)+((int)c2<<16)+((int)c3<<8)+c4;
}
