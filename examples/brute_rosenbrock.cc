// Pallas Solver
// Copyright 2015. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Author: ryan.latture@gmail.com (Ryan Latture)

#include "glog/logging.h"

// Each solver is defined in its own header file.
// include the solver you wish you use:
#include "pallas/brute.h"

// define a problem you wish to solve by inheriting
// from the pallas::GradientCostFunction interface
// and implementing the Evaluate and NumParameters methods.
class Rosenbrock : public pallas::GradientCostFunction {
public:
    virtual ~Rosenbrock() {}

    virtual bool Evaluate(const double* parameters,
                          double* cost,
                          double* gradient) const {
        const double x = parameters[0];
        const double y = parameters[1];

        cost[0] = (1.0 - x) * (1.0 - x) + 100.0 * (y - x * x) * (y - x * x);
        if (gradient != NULL) {
            gradient[0] = -2.0 * (1.0 - x) - 200.0 * (y - x * x) * 2.0 * x;
            gradient[1] = 200.0 * (y - x * x);
        }
        return true;
    }

    virtual int NumParameters() const { return 2; }
};

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    // define the starting point for the optimization
    double parameters[2] = {-1.2, 0.0};

    // set up global optimizer options only initialization
    // is need to accept the default options
    pallas::Brute::Options options;

    // initialize a summary object to hold the
    // optimization details
    pallas::Brute::Summary summary;

    // create a problem from your cost function
    pallas::GradientProblem problem(new Rosenbrock());

    // set up the subset of parameter space to test
    std::vector<pallas::Brute::ParameterRange> ranges = {pallas::Brute::ParameterRange(-3.0, 3.0, 7),
                                                         pallas::Brute::ParameterRange(-3.0, 3.0, 7)};

    // solve the problem and store the optimal position
    // in parameters and the optimization details in
    // the summary
    pallas::Solve(options, problem, ranges, parameters, &summary);

    std::cout << summary.FullReport() << std::endl;
    std::cout << "Global minimum found at:" << std::endl;
    std::cout << "\tx: " << parameters[0] << "\ty: " << parameters[1] << std::endl;

    return 0;
}