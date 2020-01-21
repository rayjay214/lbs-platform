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

    def getCustomerInfo(self, request, context):
        with self.tree_lock.gen_rlock():
            if request.eid != 0:
                node = search.find_by_attr(self.tree.root, name='id', value=request.eid)
            else:
                node = search.find_by_attr(self.tree.root, name='login_name', value=request.login_name)
            if node is None:
                return CustomerInfo()
            dev_ids = node.dev_ids
            pid = node.parent.id if node.parent is not None else 0
            info = CustomerInfo(eid=node.id, login_name=node.login_name,
                phone=node.phone, addr=node.addr, email=node.email, is_leaf=node.is_leaf,
                own_dev_num=len(node.dev_ids), total_dev_num=node.total_dev_num, pid=pid)
            for dev_id in dev_ids:
                info.dev_ids.append(dev_id)
            return info

    def getCustomerInfos(self, request_iterator, context):
        with self.tree_lock.gen_rlock():
            for cust in request_iterator:
                node = search.find_by_attr(self.tree.node, name='id', value=cust.eid)
                if node is None:
                    info = CustomerInfo(eid=node.id)
                    yield info
                dev_ids = node.dev_ids
                info = CustomerInfo(eid=node.id, login_name=node.login_name,
                    phone=node.phone, addr=node.addr, email=node.email, is_leaf=node.is_leaf,
                    own_dev_num=len(node.dev_ids), total_dev_num=node.total_dev_num)
                for dev_id in dev_ids:
                    info.dev_ids.append(dev_id)
                yield info

    def getChildrenInfo(self, request, context):
        with self.tree_lock.gen_rlock():
            node = search.find_by_attr(self.tree.root, name='id', value=request.eid)
            children = node.children
            for child in children:
                info = CustomerInfo(eid=child.id, login_name=child.login_name,
                    phone=child.phone, addr=child.addr, email=child.email, is_leaf=child.is_leaf,
                    own_dev_num=len(child.dev_ids), total_dev_num=child.total_dev_num)
                yield info

    def getAncestorsInfo(self, request, context):
        with self.tree_lock.gen_rlock():
            node = search.find_by_attr(self.tree.root, name='id', value=request.eid)
            if node is None:
                return CustomerInfo()
            ancestors = node.ancestors
            if ancestors is None:
                return CustomerInfo()
            for ances in ancestors:
                info = CustomerInfo(eid=ances.id, login_name=ances.login_name,
                    phone=ances.phone, addr=ances.addr, email=ances.email,
                    own_dev_num=len(ances.dev_ids), total_dev_num=ances.total_dev_num)
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


