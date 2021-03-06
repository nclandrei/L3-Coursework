#include "driver.h"

#include <stdio.h>
#ifndef OSX
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#include <CL/cl.h>
#else
#include <OpenCL/opencl.h>
#endif

////////////////////////////////////////////////////////////////////////////////
//                           GUID: 2147392n                                   //
//        I hereby declare that this is my own, original work.                //
////////////////////////////////////////////////////////////////////////////////
CLObject* init_driver() {
    CLObject* ocl = (CLObject*)malloc(sizeof(CLObject));
    int err;                            // error code returned from api calls

    unsigned int status[1]={0};               // number of correct results returned

    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation

    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue command_queue;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel

    cl_mem input1, input2;                       // device memory used for the input array
    cl_mem output, status_buf;                      // device memory used for the output array

    FILE* programHandle;
    size_t programSize;
    char *programBuffer;
 
    cl_uint nplatforms;
    err = clGetPlatformIDs(0, NULL, &nplatforms);
    if (err != CL_SUCCESS) {
        fprintf(stderr,"Error: Failed to get number of platform: %d!\n", err);
        exit(EXIT_FAILURE);
    }

    // Now ask OpenCL for the platform IDs:
    cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * nplatforms);
    err = clGetPlatformIDs(nplatforms, platforms, NULL);
    if (err != CL_SUCCESS) {
        fprintf(stderr,"Error: Failed to get platform IDs: %d!\n",err);
        exit(EXIT_FAILURE);

    }
#ifdef GPU
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
#else    
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
#endif    
    if (err != CL_SUCCESS)
    {
        fprintf(stderr,"Error: Failed to create a device group: %d!\n",err);
        exit(EXIT_FAILURE);

    }

    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        fprintf(stderr,"Error: Failed to create a compute context: %d!\n",err);
        exit(EXIT_FAILURE);

    }

    // Create a command command_queue
    //
    command_queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (!command_queue)
    {
        fprintf(stderr,"Error: Failed to create a command command_queue: %d!\n",err);
        exit(EXIT_FAILURE);

    }
    // get size of kernel source
    programHandle = fopen("./firmware.cl", "r");
    fseek(programHandle, 0, SEEK_END);
    programSize = ftell(programHandle);
    rewind(programHandle);

    // read kernel source into buffer
    programBuffer = (char*) malloc(programSize + 1);
    programBuffer[programSize] = '\0';
    fread(programBuffer, sizeof(char), programSize, programHandle);
    fclose(programHandle);

    // create program from buffer
    program = clCreateProgramWithSource(context, 1, (const char**) &programBuffer, &programSize, &err);
    free(programBuffer);
    if (!program)
    {
        fprintf(stderr,"Error: Failed to create compute program: %d!\n",err);
        exit(EXIT_FAILURE);

    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        fprintf(stderr,"Error: Failed to build program executable: %d!\n",err);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        fprintf(stderr,"%s\n", buffer);
        exit(EXIT_FAILURE);
    }

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "firmware", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        fprintf(stderr,"Error: Failed to create compute kernel: %d!\n",err);
        exit(EXIT_FAILURE);

    }
    ocl->context = context;
    ocl->command_queue = command_queue;
    ocl->kernel = kernel;
    ocl->program= program;
    ocl->device_id = device_id;

//===============================================================================================================================================================  
// START of assignment code section 
    err = pthread_mutex_init(&ocl->device_lock, NULL);
    if (err != 0) {
        fprintf(stderr, "Error: Failed to initialize mutex lock!%d\n", err);
    }
// END of assignment code section 
//===============================================================================================================================================================  
    
    return ocl;
}

int shutdown_driver(CLObject* ocl) {
    int err = clReleaseProgram(ocl->program);
     if (err != CL_SUCCESS) {
            fprintf(stderr,"Error: Failed to release Program: %d!\n",err);
        exit(EXIT_FAILURE);
     }
    err = clReleaseKernel(ocl->kernel);
     if (err != CL_SUCCESS) {
            fprintf(stderr,"Error: Failed to release Kernel: %d!\n",err);
        exit(EXIT_FAILURE);
     }
    err = clReleaseCommandQueue(ocl->command_queue);
     if (err != CL_SUCCESS) {
            fprintf(stderr,"Error: Failed to release Command Queue: %d!\n",err);
        exit(EXIT_FAILURE);
     }
    err = clReleaseContext(ocl->context);
     if (err != CL_SUCCESS) {
            fprintf(stderr,"Error: Failed to release Context: %d!\n",err);
        exit(EXIT_FAILURE);
     }
//===============================================================================================================================================================  
// START of assignment code section      
    err = pthread_mutex_destroy(&ocl->device_lock);
    if (err != 0) {
        fprintf(stderr, "Error: Failed to destroy CLObject's mutex!%d\n", err);
        exit(EXIT_FAILURE);
    }
    err = clReleaseDevice(ocl->device_id);
    if (err != CL_SUCCESS) {
	    fprintf(stderr, "Error: Failed to release Device: %d!\n", err);
        exit(EXIT_FAILURE);
    }
// END of assignment code section 
//===============================================================================================================================================================  
     
    free(ocl);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int run_driver(CLObject* ocl,unsigned int buffer_size,  int* input_buffer_1, int* input_buffer_2, int* output_buffer) {
    long long unsigned int tid = ocl->thread_num;
#if VERBOSE_MT>2    
     printf("run_driver thread: %llu\n",tid);
#endif
     int err;                            // error code returned from api calls
     int status[1]={-1};               // number of correct results returned
     unsigned int max_iters;
     max_iters = MAX_ITERS;

     size_t global;                      // global domain size for our calculation
     size_t local;                       // local domain size for our calculation

     cl_mem input1, input2;                       // device memory used for the input array
     cl_mem output, status_buf;                      // device memory used for the output array

     // Get the maximum work group size for executing the kernel on the device
     err = clGetKernelWorkGroupInfo(ocl->kernel, ocl->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
     if (err != CL_SUCCESS) {
         fprintf(stderr,"Error: Failed to retrieve kernel work group info! %d\n", err);
         exit(EXIT_FAILURE);
     }

     global = buffer_size; // create as meany threads on the device as there are elements in the array

//===============================================================================================================================================================  
// START of assignment code section 

    // You must make sure the driver is thread-safe by using the appropriate POSIX mutex operations
    // You must also check the return value of every API call and handle any errors 

    // Create the buffer objects to link the input and output arrays in device memory to the buffers in host memory
   
    err = pthread_mutex_lock(&ocl->device_lock); 
    if (err != 0) {
        fprintf(stderr, "Error: Failed to lock mutex! %d\n", err);
        exit(EXIT_FAILURE);
    }

    input1 = clCreateBuffer (ocl->context, CL_MEM_READ_ONLY, buffer_size * sizeof(int), NULL, &err);
    if (!input1) {
	    fprintf(stderr, "Error: Failed to create input buffer 2! %d\n", err);
	    exit(EXIT_FAILURE);
    }

    input2 = clCreateBuffer (ocl->context, CL_MEM_READ_ONLY, buffer_size * sizeof(int), NULL, &err);
    if (!input2) {
	    fprintf(stderr, "Error: Failed to create input buffer 1! %d\n", err);
	    exit(EXIT_FAILURE);
    }

    output = clCreateBuffer (ocl->context, CL_MEM_WRITE_ONLY, buffer_size * sizeof(int), NULL, &err);
    if (!output) {
	    fprintf(stderr, "Error: Failed to create output buffer! %d\n", err);
	    exit(EXIT_FAILURE);
    }

    status_buf = clCreateBuffer(ocl->context, CL_MEM_WRITE_ONLY, sizeof(int), NULL, &err);
    if (!status_buf) {
	    fprintf(stderr, "Error: Failed to create status buffer! %d\n", err);
	    exit(EXIT_FAILURE);
    }
        
    // Write the data in input arrays into the device memory 
 
    err = clEnqueueWriteBuffer (ocl->command_queue, input1, CL_TRUE, 0, buffer_size * sizeof(int), input_buffer_1, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
	    fprintf(stderr, "Error: Failed to write to first source array! %d\n", err);
 	    exit(EXIT_FAILURE);
    }

    err = clEnqueueWriteBuffer (ocl->command_queue, input2, CL_TRUE, 0, buffer_size * sizeof(int), input_buffer_2, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
	    fprintf(stderr, "Error: Failed to write to second source array! %d\n", err);
 	    exit(EXIT_FAILURE);
    }

    // Set the arguments to our compute kernel
    
    err = clSetKernelArg(ocl->kernel, 0, sizeof(cl_mem), &input1);
    if (err != CL_SUCCESS) {
         fprintf(stderr,"Error: Failed to set first kernel argument! %d\n", err);
         exit(EXIT_FAILURE);
    }

    err = clSetKernelArg(ocl->kernel, 1, sizeof(cl_mem), &input2);
    if (err != CL_SUCCESS) {
         fprintf(stderr,"Error: Failed to set second kernel argument! %d\n", err);
         exit(EXIT_FAILURE);
    }

    err = clSetKernelArg(ocl->kernel, 2, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS) {
         fprintf(stderr,"Error: Failed to set third kernel argument! %d\n", err);
         exit(EXIT_FAILURE);
    }

    err = clSetKernelArg(ocl->kernel, 3, sizeof(cl_mem), &status_buf);
    if (err != CL_SUCCESS) {
         fprintf(stderr,"Error: Failed to set fourth kernel argument! %d\n", err);
         exit(EXIT_FAILURE);
    }

    err = clSetKernelArg(ocl->kernel, 4, sizeof(const unsigned int), &buffer_size);
    if (err != CL_SUCCESS) {
         fprintf(stderr,"Error: Failed to set fifth kernel argument! %d\n", err);
         exit(EXIT_FAILURE);
    }
   
    for (int count = 0; count < max_iters; ++count) {  

        // Execute the kernel, i.e. tell the device to process the data using the given global and local ranges
        err = clEnqueueNDRangeKernel(ocl->command_queue, ocl->kernel, 1, NULL, &global, &local, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr,"Error: Failed to execute the kernel! %d\n", err);
            exit(EXIT_FAILURE);
        }

        // Wait for the command commands to get serviced before reading back results. This is the device sending an interrupt to the host   
        err = pthread_mutex_unlock(&ocl->device_lock);
        if (err != 0) {
            fprintf(stderr, "Error: Failed to release mutex! %d\n", err);
            exit(EXIT_FAILURE);
        }

        clFinish(ocl->command_queue);

        err = pthread_mutex_lock(&ocl->device_lock);
        if (err != 0) {
            fprintf(stderr, "Error: Failed to lock mutex! %d\n", err);
            exit(EXIT_FAILURE);
        }

        // Check the status (try max_iters times until the status is equal to 0)
        err = clEnqueueReadBuffer (ocl->command_queue, status_buf, CL_TRUE, 0, sizeof(int), &status, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr, "Error: Failed to read back the status!%d\n", err);
            exit(EXIT_FAILURE);
        }

        // If status is 0, we don't need to iterate anymore
        if (status[0] == 0) {
            break;
        }
    }

    // When the status is 0, read back the results from the device to verify the output 
    if (status[0] == 0) {
        err = clEnqueueReadBuffer (ocl->command_queue, output, CL_TRUE, 0, buffer_size * sizeof(int), output_buffer, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr, "Error: Failed to read back the output array!%d\n", err);
            exit(EXIT_FAILURE);
        }
    }
    else {
#ifdef VERBOSE_MT
        printf("Could not read back results from device! Status = %d\n", status[0]);
#endif
    }

    // Shutdown and cleanup

    err = clReleaseMemObject(input1);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error: Failed to release first input memory object! %d\n", err);
        exit(EXIT_FAILURE);
    }

    err = clReleaseMemObject(input2); 
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error: Failed to release second input memory object! %d\n", err);
        exit(EXIT_FAILURE);
    }

    err = clReleaseMemObject(output); 
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error: Failed to release output object! %d\n", err);
        exit(EXIT_FAILURE);
    }

    err = clReleaseMemObject(status_buf); 
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error: Failed to release status object! %d\n", err);
        exit(EXIT_FAILURE);
    }

    err = pthread_mutex_unlock(&ocl->device_lock);
    if (err != 0) {
        fprintf(stderr, "Error: Failed to release mutex! %d\n", err);
        exit(EXIT_FAILURE);
    }

// END of assignment code section 
//===============================================================================================================================================================  
    return *status;

}
