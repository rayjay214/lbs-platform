from globals import g_logger, g_cfg
import grpc
from customer_tree_pb2 import CustomerInfo
import customer_tree_pb2_grpc
from concurrent import futures
import threading
from anytree import search

class CTreeServicer(customer_tree_pb2_grpc.CTreeServicer):
    def __init__(self, tree, tree_lock):
        self.tree = tree
        self.tree_lock = tree_lock

    def getCustomInfo(self, request, context):
        with self.tree_lock.gen_rlock():
            node = search.find_by_attr(self.tree.root, name='id', value=request.eid)
            return CustomerInfo(eid=node.id, login_name=node.login_name,
                    phone=node.phone, addr=node.addr, email=node.email, is_leaf=node.is_leaf,
                    own_dev_num=len(node.dev_ids), total_dev_num=node.total_dev_num)

    def getChildrenInfo(self, request, context):
        with self.tree_lock.gen_rlock():
            node = search.find_by_attr(self.tree.root, name='id', value=request.eid)
            children = node.children
            for child in children:
                info = CustomerInfo(eid=child.id, login_name=child.login_name,
                    phone=child.phone, addr=child.addr, email=child.email, is_leaf=child.is_leaf,
                    own_dev_num=len(child.dev_ids), total_dev_num=child.total_dev_num)
                yield info



class GrpcThread(threading.Thread):
    def __init__(self, ctree, tree_lock):
        threading.Thread.__init__(self)
        self.ctree = ctree
        self.tree_lock = tree_lock

    def run(self):
        server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
        customer_tree_pb2_grpc.add_CTreeServicer_to_server(
            CTreeServicer(self.ctree, self.tree_lock), server)
        server.add_insecure_port('[::]:50051')
        server.start()
        server.wait_for_termination()


