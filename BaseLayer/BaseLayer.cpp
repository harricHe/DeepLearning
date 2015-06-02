#include <cmath>
#include "BaseLayer.h"

BaseLayer::BaseLayer(int inputDim0, int outputDim0, ActivationType actType0) {
    inputDim = inputDim0;
    outputDim = outputDim0;
    actType = actType0;
    initializeWeight();
};

void BaseLayer::initializeWeight() {
    W = std::make_shared<arma::mat>(outputDim,inputDim);
    B = std::make_shared<arma::vec>(outputDim);
    W->randu(outputDim,inputDim);
    B->randu(outputDim);
    (*W) -= 0.5;
    (*B) -= 0.5;

    if (actType == sigmoid) {
        (*W) *=4*sqrt(6.0/(inputDim+outputDim));
        (*B) *=4*sqrt(6.0/(inputDim+outputDim));
    } else if (actType == softmax) {
        (*W) *=sqrt(6.0/(inputDim+outputDim));
        (*B) *=sqrt(6.0/(inputDim+outputDim));

    }

}

void BaseLayer::save(std::string filename) {
    W->save(filename+"_W.dat",arma::raw_ascii);
    B->save(filename+"_B.dat",arma::raw_ascii);

}

void BaseLayer::updatePara(std::shared_ptr<arma::vec> delta_in) {

    arma::vec deriv;
    arma::vec delta;
    delta_out = std::make_shared<arma::vec>(outputDim);
    if (actType == sigmoid || actType == softmax) {
        deriv = (1 - (*outputY)) % (*outputY);
        delta = (delta_in % deriv );
        (*delta_out) = delta * (*W).st();
    } else if ( actType == tanh) {
        deriv = (1 - (*outputY)) % (*outputY);
        delta = (delta_in % deriv );
        (*delta_out) = (delta_in % deriv) * (*W).st();
    } else if ( actType == linear) {
        delta_out.ones();
    }

    (*B) -= delta;
    (*W) -= delta * (*inputX);

}
void BaseLayer::activateUp(std::shared_ptr<arma::mat> input) {
    outputY = std::make_shared<arma::mat>(input->n_rows,outputDim);
    std::shared_ptr<arma::mat> &p=outputY;
// first get the projection
    (*p) = (*input) * (*W).st();

    for (int i = 0; i < inputX->n_rows; i++) p->row(i) += (*B).st();
// then do the activation
    arma::mat maxVal = arma::max(*p,1);
    switch(actType) {
    case softmax:
//        p->print();
//        maxVal.print();
        for (int i = 0; i < inputX->n_rows; i++) {
            for (int j = 0; j < outputDim; j++) {
                (*p)(i,j)-= maxVal(i);
            }
        }
        (*p).transform([](double val) {
            return exp(val);
        });

        double normfactor;
        for (int i = 0; i < inputX->n_rows; i++) {
            normfactor = 0.0;
            for (int j = 0; j < outputDim; j++) {
                normfactor+=p->at(i,j);
            }
            for (int j = 0; j < outputDim; j++) {
                (*p)(i,j)/=normfactor;
            }
        }
//    std::cout << normfactor << std::endl;
//    p->print();
        break;
    case sigmoid:
//        p->print();
        (*p).transform([](double val) {
            return 1.0/(1.0+exp(-val));
        }) ;
        break;
    case linear:
        break;
    }
}






