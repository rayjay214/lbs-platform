import grpc
import customer_tree_pb2
import customer_tree_pb2_grpc

with grpc.insecure_channel('localhost:50051') as channel:
    stub = customer_tree_pb2_grpc.CTreeStub(channel)
    customer_info = customer_tree_pb2.CustomerInfo(eid=10000)
    cust = stub.getCustomInfoByID(customer_info)
    print(cust)
