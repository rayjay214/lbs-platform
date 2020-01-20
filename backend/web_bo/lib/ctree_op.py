import grpc
import customer_tree_pb2
import customer_tree_pb2_grpc

class CtreeOp():
    def __init__(self, cfg):
        self.server_ip = cfg['server_ip']
        self.server_port = cfg['server_port']

    def getCustomInfoByEid(self, eid):
        with grpc.insecure_channel('{}:{}'.format(self.server_ip, self.server_port)) as channel:
            stub = customer_tree_pb2_grpc.CTreeStub(channel)
            customer_info = customer_tree_pb2.CustomerInfo(eid=eid)
            cust = stub.getCustomerInfo(customer_info)
            return cust

    def getChildrenInfoByEid(self, eid):
        channel = grpc.insecure_channel('{}:{}'.format(self.server_ip, self.server_port))
        stub = customer_tree_pb2_grpc.CTreeStub(channel)
        customer_info = customer_tree_pb2.CustomerInfo(eid=eid)
        children = stub.getChildrenInfo(customer_info)
        return children, channel

    def isAncestor(self, ancestor, descendant):
        grpc.insecure_channel('{}:{}'.format(self.server_ip, self.server_port)) as channel:
            stub = customer_tree_pb2_grpc.CTreeStub(channel)
            customer_info = customer_tree_pb2.CustomerInfo(eid=descendant)
            ancestors = stub.getAncestorsInfo()
            for cust in ancestors:
                if cust.eid == ancestor:
                    return True
            return False
