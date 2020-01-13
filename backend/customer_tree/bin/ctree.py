from anytree import Node, RenderTree
from anytree import AnyNode
from anytree import search
from globals import g_logger, g_cfg

class CustomerTree():
    def __init__(self, db, rwlock):
        self.root = None
        self.data_source = db
        self.lock = rwlock

    def add_devnum_upwards(self, node):
        while node is not None:
            node.total_dev_num = node.total_dev_num + 1
            node = node.parent

    def init_fromdb(self):
        ents = self.data_source.load_all_ent()
        for ent in ents:
            if ent['pid'] == 0:
                self.root = AnyNode(name=ent['login_name'], id=ent['eid'], dev_ids=set(), total_dev_num=0, parent=None)
                continue
            myparent = search.find_by_attr(self.root, name='id', value=ent['pid'])
            node = AnyNode(name=ent['login_name'], id=ent['eid'], dev_ids=set(), total_dev_num=0, parent=myparent)
        devices = self.data_source.load_all_device()
        cache_node = {}
        for device in devices:
            owner = cache_node.get(device['eid'])
            if owner is None:
                owner = search.find_by_attr(self.root, name='id', value=device['eid'])
                if owner is None:
                    g_logger.info('can not find eid {}'.format(device['eid']))
                    continue
                owner.dev_ids.add(device['dev_id'])
                self.add_devnum_upwards(owner)
                cache_node[device['eid']] = owner
            else:
                owner.dev_ids.add(device['dev_id'])
                self.add_devnum_upwards(owner)

    def dump_tree(self):
        g_logger.info(RenderTree(self.root))

    def insert_device(self, msg):
        print('insert device')

    def insert_ent(self, msg):
        print('insert ent')

    def update_device(self, msg):
        print('update device')

    def update_ent(self, msg):
        print('update ent')


