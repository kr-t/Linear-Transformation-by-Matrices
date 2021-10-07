#include "data_channel.hpp"


/* Main function */
int main()
{
    /* Open input file of matrices */
    std::ifstream inputMatrices("input_matrices.txt");

    if (inputMatrices.is_open())
    {
        /* Store number of matrices */
        unsigned int numberOfMatrices;
        inputMatrices >> numberOfMatrices;

        /* Store matrices in numberOfMatrices * 4 * 4 tensor */
        auto matrices = create_tensor(numberOfMatrices, MATRIX_DIMENSION_SIZE, MATRIX_DIMENSION_SIZE);

        for (unsigned int i = 0; i < numberOfMatrices; i++)
        {
            for (unsigned int y = 0; y < MATRIX_DIMENSION_SIZE; y++)
            {
                for (unsigned int k = 0; k < MATRIX_DIMENSION_SIZE; k++)
                {
                    inputMatrices >> matrices[i][y][k];
                }
            }
        }

        inputMatrices.close();

        /* Open input file of vectors */
        std::ifstream inputVectors("input_points.txt");

        if (inputVectors.is_open())
        {
            /* Store number of vectors */
            unsigned int numberOfVectors;
            inputVectors >> numberOfVectors;

            /* Store vectors in numberOfVectors * (3 + 1) matrix.
               Create additional row for simpler multiplication */
            auto vectors = create_matrix(numberOfVectors, (VECTOR_DIMENSION_SIZE + 1));

            for (unsigned int i = 0; i < numberOfVectors; i++)
            {
                for (unsigned int y = 0; y < VECTOR_DIMENSION_SIZE; y++)
                {
                    inputVectors >> vectors[i][y];
                }
                /* Fill the last row with ones */
                vectors[i][VECTOR_DIMENSION_SIZE] = 1;
            }

            inputVectors.close();

            /* Start threads, data channel and data flow */
            startDataFlow(numberOfMatrices, numberOfVectors, vectors, matrices);
        }
    }

    return 0;
}
