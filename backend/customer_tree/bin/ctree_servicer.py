from globals import g_logger, g_cfg
import grpc
import customer_tree_pb2
import customer_tree_pb2_grpc
from concurrent import futures
import threading
from anytree import search

class CTreeServicer(customer_tree_pb2_grpc.CTreeServicer):
    def __init__(self, tree, tree_lock):
        self.tree = tree
        self.tree_lock = tree_lock

    def getCustomInfoByID(self, request, context):
        with self.tree_lock.gen_rlock():
            node = search.find_by_attr(self.tree.root, name='id', value=request.eid)
            return customer_tree_pb2.CustomerInfo(
                eid=node.id,
                login_name=node.name)


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


