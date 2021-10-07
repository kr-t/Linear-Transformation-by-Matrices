#include "data_channel.hpp"

/* Static function declarations */
static void multiply(matrix matrix, std::vector<int> &vector);
static void pipeline(unsigned int numberOfVectors, matrix matrix, Pipe<std::vector<int>> &from, Pipe<std::vector<int>> &to);
static void output(unsigned int numberOfVectors, Pipe<std::vector<int>> &pipe);

/* Static function definitions */
/* Function to multiply matrix with vector */
static void multiply(matrix matrix, std::vector<int> &vector)
{
    /* Variable to store vector with 4 elements filled with zeros */
    std::vector<int> retVal((VECTOR_DIMENSION_SIZE + 1), 0);

    /* Multiply input matrix with vector */
    for (unsigned int i = 0; i < MATRIX_DIMENSION_SIZE; i++)
    {
        for (unsigned int y = 0; y < MATRIX_DIMENSION_SIZE; y++)
        {
            retVal[i] += vector[y] * matrix[i][y];
        }
    }

    /* Return vector */
    vector = retVal;
}

/* Function to complete data flow's pipeline logic */
static void pipeline(unsigned int numberOfVectors, matrix matrix, Pipe<std::vector<int>> &from, Pipe<std::vector<int>> &to)
{
    /* Create vector to be pushed */
    std::vector<int> retVal((VECTOR_DIMENSION_SIZE + 1), 0);

    /* Get previous element of the flow, complete multiplication and send
       the calculated element to the next matrix*/
    for (unsigned int i = 0; i < numberOfVectors; i++)
    {
        retVal = from.pop();      /* Get previous element */
        multiply(matrix, retVal); /* Complete multiplication */
        to.push(retVal);          /* Send element to the next station */
    }
}

/* Function to create ouput file */
static void output(unsigned int numberOfVectors, Pipe<std::vector<int>> &pipe)
{
    /* Open output file */
    std::ofstream outputFile("output.txt");

    if (outputFile.is_open())
    {
        std::vector<int> retVal((VECTOR_DIMENSION_SIZE + 1), 0);

        /* Write ouput and close the file */
        for (unsigned int i = 0; i < numberOfVectors; i++)
        {
            retVal = pipe.pop(); /* Get the element after the last multiplication */
            outputFile << retVal[0] << " " << retVal[1] << " " << retVal[2] << std::endl;
        }

        outputFile.close();
    }
}

/* Extern function definitions */
/* Function to create 2D vector of int values */
extern matrix create_matrix(int y, int x, int value)
{
    return matrix(y, std::vector<int>(x, value));
}

/* Function to create 3D vector of int values */
extern tensor create_tensor(int z, int y, int x, int value)
{
    return tensor(z, matrix(y, std::vector<int>(x, value)));
}

/* Function to open and close threads */
extern void startDataFlow(unsigned int numberOfMatrices, unsigned int numberOfVectors, matrix vectors, tensor matrices)
{
    /* Create thread and pipe variables */
    std::vector<std::thread> dataflow(numberOfMatrices);
    std::vector<Pipe<std::vector<int>>> pipes(numberOfMatrices + 1);

    /* Push input vectors to the pipe */
    for (unsigned int i = 0; i < numberOfVectors; i++)
    {
        pipes[0].push(vectors[i]);
    }

    /* Start threads for every matrix multiplication */
    for (unsigned int i = 0; i < numberOfMatrices; i++)
    {
        dataflow[i] = std::thread([&](int j) { pipeline(numberOfVectors, matrices[j], pipes[j], pipes[j + 1]); }, i);
    }

    /* Start thread to write output */
    std::thread out(output, numberOfVectors, std::ref(pipes[numberOfMatrices]));

    /* Close all threads */
    for (unsigned int i = 0; i < numberOfMatrices; i++)
    {
        dataflow[i].join();
    }

    out.join();
}
